#define _CRT_SECURE_NO_WARNINGS

#include "Stabilizer.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <functional>
#include "qdebug.h"

#define AVERAGE_WINDOW 3

using namespace std;
using namespace cv;

int GetSize(int width, int height);
Rect findMinRect(const Mat1b& src);
Mat concatenateMatrix(Mat first, Mat second);

Stabilizer::Stabilizer()
{
}


Stabilizer::~Stabilizer()
{
}

void Stabilizer::Stabilize(VideoCapture *cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback, bool just_detect)
{
	cv::Mat mask_accumulated;
	vector<Mat> transforms;
	vector<Mat> transforms_pass_2;

	// First pass uses feature detection and optical flow to approximate stabilization transformations
	transforms = StabilizePass1(cap, mask, callback, just_detect);

	if (just_detect)
		return;
	
	// Second pass refines the alignment using area-based intensity similarities
	transforms_pass_2 = StabilizePass2(cap, callback, transforms, mask_accumulated);

	// Third pass crops the final video to remove borders created by the alignment transformations
	StabilizePass3(cap, mask_accumulated, callback, transforms, transforms_pass_2);
}

void Stabilizer::StabilizePass3(VideoCapture *cap, cv::Mat mask, std::function<void(cv::Mat frame)> callback, vector<cv::Mat> transforms, vector<cv::Mat> transforms_pass_2)
{
	
	Mat current;
	VideoWriter output_video;
	int current_frame_index = -1;
	cap->set(CV_CAP_PROP_POS_FRAMES, 0);

	// Calculate crop rect
	Rect crop_rect = findMinRect(~mask);
	Size S = crop_rect.size();

	//CV_FOURCC('P', 'I', 'M', '1')	
	output_video.open("C:\\Users\\mattc\\Desktop\\temp\\out.avi", -1, cap->get(CV_CAP_PROP_FPS), S, true);

	while (cap->read(current))
	{
		if (current_frame_index != -1)
		{
			Mat warped_1, warped_2, out;
			warpAffine(current, warped_1, transforms[current_frame_index], current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
			warpAffine(warped_1, warped_2, transforms_pass_2[current_frame_index], current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
			warped_2.copyTo(out);
			rectangle(out, crop_rect, Scalar(0, 255, 0), 1);
			callback(out);
			Mat cropped = warped_2(crop_rect);
			output_video.write(cropped);
		}
		current_frame_index++;
	}

}

vector<Mat> Stabilizer::StabilizePass2(VideoCapture *cap, ::function<void(cv::Mat frame)> callback, vector<Mat> transforms, cv::Mat &mask_accumulated)
{
	cap->set(CV_CAP_PROP_POS_FRAMES, 0);

	Mat current, previous;
	Mat previous_warped;
	vector<cv::Mat> ecc_transforms;

	int transform_index = 0;

	if (!cap->read(previous))
		return ecc_transforms;
	
	previous.copyTo(previous_warped);

	while (cap->read(current))
	{	
		Mat warp_matrix = Mat::eye(2, 3, CV_32F);
		Mat T = transforms[transform_index];
		Mat dest;

		// Create a mask for calculating borders
		Mat mask, mask_warped_stage_1, mask_warped_stage_2;
		mask = Mat(Size(current.cols, current.rows), CV_8UC1, Scalar(0));
		mask.setTo(Scalar::all(255));

		// Apply pass-1 warp
		warpAffine(current, dest, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
		warpAffine(mask, mask_warped_stage_1, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
		
		// Calculate pass-2 warp
		Mat previous_warped_gray, dest_gray;
		cvtColor(previous_warped, previous_warped_gray, COLOR_BGR2GRAY);
		cvtColor(dest, dest_gray, COLOR_BGR2GRAY);
		findTransformECC(previous_warped_gray, dest_gray, warp_matrix, MOTION_AFFINE);
		Mat dest_temp;
		dest.copyTo(dest_temp);
		warpAffine(dest, dest_temp, warp_matrix, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
		warpAffine(mask_warped_stage_1, mask_warped_stage_2, warp_matrix, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);
		dest_temp.copyTo(dest);

		ecc_transforms.push_back(warp_matrix);

		// Apply current mask to accumulated mask
		if (!mask_accumulated.rows) {
			mask_warped_stage_2.copyTo(mask_accumulated);
		}
		for (int x = 0; x < mask_warped_stage_2.cols; x++)
		{
			for (int y = 0; y < mask_warped_stage_2.rows; y++)
			{
				if (mask_warped_stage_2.at<uchar>(Point(x, y)) < 255)
				{
					mask_accumulated.at<uchar>(Point(x, y)) = 0;
				}
			}
		}

		callback(dest);
		transform_index++;
		current.copyTo(previous);
		
	}

	return ecc_transforms;
}

vector<cv::Mat> Stabilizer::StabilizePass1(VideoCapture *cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback, bool just_detect)
{
	Mat current, current_gray;
	Mat previous, previous_gray;
	Mat warped_mask;
	Mat previous_dest;
	vector <cv::Mat> transforms;
	//VideoWriter outputVideo;
	bool ridgid_transform_pixels = false;

	// We'll apply the same affine transformation to the mask as we apply to each frame. This ensures
	// the mask "follows" the correct areas as the video pans and tilts.
	mask.copyTo(warped_mask);

	// Holds the last known good transform. We'll use this if estimateRigidTransform is not successful.
	Mat last_T;

	// Reset the frame pointer and load the first frame
	cap->set(CV_CAP_PROP_POS_FRAMES, 0);

	if (!cap->read(previous))
		return transforms;

	cvtColor(previous, previous_gray, COLOR_BGR2GRAY);

	Size S = Size((int)cap->get(CV_CAP_PROP_FRAME_WIDTH), (int)cap->get(CV_CAP_PROP_FRAME_HEIGHT));
	
	// for MPEG:
	//CV_FOURCC('P', 'I', 'M', '1')	
	//outputVideo.open("C:\\Users\\mattc\\Desktop\\temp\\out.avi", -1, cap->get(CV_CAP_PROP_FPS), S, true);

	while (cap->read(current))
	{
		cvtColor(current, current_gray, COLOR_BGR2GRAY);

		// vector from prev to cur
		vector <Point2f> previous_corner, current_corner;
		vector <Point2f> previous_corner2, current_corner2;
		vector <Point2f> previous_corner3, current_corner3;
		vector <Point2f> rejected_origins;
		vector <Point2f> rejected_points;
		vector <int> distances;
		vector <uchar> status;
		vector <float> err;
		double length_sum = 0;

		goodFeaturesToTrack(previous_gray, previous_corner, 50, 0.001, 15, warped_mask, 8, true);
		calcOpticalFlowPyrLK(previous_gray, current_gray, previous_corner, current_corner, status, err);
		
		// weed out bad matches (pass 1)
		for (size_t i = 0; i < status.size(); i++) {
			if (status[i]) {
				// ensure point still lies within the mask
				//if (current_corner[i].x > warped_mask.cols || current_corner[i].y > warped_mask.rows
				//	|| current_corner[i].x < 0 || current_corner[i].y < 0)
				//	continue;
				//if (warped_mask.at<uchar>(Point(current_corner[i].x, current_corner[i].y)) != 255)
				//	continue;

				// calculate distance moved between previous and current
				int a = previous_corner[i].y - current_corner[i].y;
				int b = previous_corner[i].x - current_corner[i].x;
				double distance = abs(sqrt(a*a + b*b));
				length_sum += distance;
				distances.push_back(distance);
				previous_corner2.push_back(previous_corner[i]);
				current_corner2.push_back(current_corner[i]);
			}
			else {
				rejected_origins.push_back(previous_corner[i]);
				rejected_points.push_back(current_corner[i]);
			}
		}

		// weed out bad matches (pass 2)
		double mean = length_sum / distances.size();
		double threshold = mean * 2;
		for (size_t i = 0; i < distances.size(); i++)
		{
			if (distances[i] <= threshold)
			{
				previous_corner3.push_back(previous_corner2[i]);
				current_corner3.push_back(current_corner2[i]);
			}
			else
			{
				rejected_origins.push_back(previous_corner2[i]);
				rejected_points.push_back(current_corner2[i]);
			}
		}


		// Estimate rigid transform between the current frame and previous frame. We'll either
		// do this by comparing the feature points or the pixels directly.
		Mat T = estimateRigidTransform(previous_corner3, current_corner3, false);
	
		if (T.data == NULL) {
			last_T.copyTo(T);
		}

		T.copyTo(last_T);
		transforms.push_back(T);

		// decompose T
		double dx = T.at<double>(0, 2);
		double dy = T.at<double>(1, 2);
		double da = atan2(T.at<double>(1, 0), T.at<double>(0, 0));
		
		// Create a Mat for holding the preview (out) frame. Draw crosshairs on the tracked features.
		Mat out, out_dest;
		current.copyTo(out);
		for (int i = 0; i < current_corner3.size(); i++)
		{
			Point p = current_corner3[i];
			Point o = previous_corner3[i];
			line(out, Point(p.x - 7, p.y), Point(p.x + 7, p.y), Scalar(0, 255, 0), 1);
			line(out, Point(p.x, p.y - 7), Point(p.x, p.y + 7), Scalar(0, 255, 0), 1);
			line(out, Point(o.x, o.y), Point(p.x, p.y), Scalar(255, 255, 0), 1);
		}

		// Draw rejected points
		for (int i = 0; i < rejected_points.size(); i++)
		{
			Point p = rejected_points[i];
			Point o = rejected_origins[i];
			line(out, Point(p.x - 7, p.y), Point(p.x + 7, p.y), Scalar(0, 0, 255), 1);
			line(out, Point(p.x, p.y - 7), Point(p.x, p.y + 7), Scalar(0, 0, 255), 1);
			line(out, Point(o.x, o.y), Point(p.x, p.y), Scalar(0, 0, 190), 1);
		}

		// Warp the current (pristine) video frame to dest
		// Warp the current (annotated) video frame to out_dest
		// Warp mask into temp_mask then copy back to warped_mask
		// TODO: Warped mask always lags behind by one frame, since the the warp is applied *after* it has
		//       already been used to detect features. Not sure how to to address this. Maybe store the
		//       transforms and do another pass?
		Mat dest;
		Mat temp_mask;
		warpAffine(current, dest, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP); //INTER_LANCZOS4
		out.copyTo(out_dest);
        warpAffine(out, out_dest, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP); //INTER_LANCZOS4
		warpAffine(mask, temp_mask, T, current.size());// WARP_INVERSE_MAP);
		temp_mask.copyTo(warped_mask);

		//outputVideo.write(dest);
		callback(out_dest);
		current.copyTo(previous);

		if (just_detect)
			return transforms;
	}

	return transforms;
}

Rect Stabilizer::LargestInscribedRect(cv::Mat mask)
{
	int height = mask.rows;
	int width = mask.rows;
	vector<vector<int>>sizes(mask.cols, vector<int>(mask.rows));
	vector<vector<int>>squares(mask.cols, vector<int>(mask.rows));

	int row;
	int col;

	// process bottom boundary of the mask
	row = (height - 1);

	for (col = 0; col < width; col++)
	{
		if (mask.at<uchar>(Point(col, row)) == 255)
		{
			squares[col][row] = 1;
		}
	}

	// process right boundary of the mask
	col = (width - 1);

	for (row = 0; row < height; row++)
	{
		if (mask.at<uchar>(Point(col,row)) == 255)
		{
			squares[row][col] = 1;
		}
	}

	// process internal pixels of the mask
	for (
		row = (height - 2);
		row >= 0;
		row--)
	{
		for (
			col = (width - 2);
			col >= 0;
			col--)
		{
			if (mask.at<uchar>(Point(col,row))==255)
			{
				int a = squares[row][col + 1];
				int b = squares[row + 1][col];
				int c = squares[row + 1][col + 1];
				
				squares[row][col] = (min(min(a, b), c) + 1);
			}
		}
	}
	
	int square;
	int maxSquare = 0;

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			square = squares[row][col];

			sizes[row][col] = GetSize(square, square);

			if (square > maxSquare)
			{
				maxSquare = square;
			}
		}
	}

	// find largest rectangles with width >= height
	vector<int>height2width(maxSquare + 1);

	vector<vector<int>>widths(height, vector<int>(width));
	vector<vector<int>>heights(height, vector<int>(width));

	int maxSize;
	int rectWidth;
	int rectHeight;
	int size;

	for (
		row = 0;
		row < height;
		row++)
	{
		for (int s = 0; s <= maxSquare; s++)
		{
			height2width[s] = 0;
		}

		for (
			col = (width - 1);
			col >= 0;
			col--)
		{
			square = squares[row][col];

			if (square > 0)
			{
				maxSize = sizes[row][col];

				for (rectHeight = square; rectHeight > 0; rectHeight--)
				{
					rectWidth = height2width[rectHeight];

					rectWidth = max(
						rectWidth + 1,
						square);

					height2width[rectHeight] = rectWidth;

					size = GetSize(rectWidth, rectHeight);

					if (size >= maxSize)
					{
						maxSize = size;
						widths[row][col] = rectWidth;
						heights[row][col] = rectHeight;
					}
				}

				sizes[row][col] = maxSize;
			}

			for (
				int s = (square + 1);
				s <= maxSquare;
				s++)
			{
				// widths larger that 'square' will not be available
				height2width[s] = 0;
			}
		}
	}

	// find largest rectangles with width < height
	vector<int>width2height(maxSquare + 1);
	for (
		col = 0;
		col < width;
		col++)
	{
		for (int s = 0; s <= maxSquare; s++)
		{
			width2height[s] = 0;
		}

		for (
			row = (height - 1);
			row >= 0;
			row--)
		{
			square = squares[row][col];

			if (square > 0)
			{
				maxSize = sizes[row][col];

				for (rectWidth = square; rectWidth > 0; rectWidth--)
				{
					rectHeight = width2height[rectWidth];

					rectHeight = max(
						rectHeight + 1,
						square);

					width2height[rectWidth] = rectHeight;

					size = GetSize(rectWidth, rectHeight);

					if (size > maxSize)
					{
						maxSize = size;
						widths[row][col] = rectWidth;
						heights[row][col] = rectHeight;
					}
				}

				sizes[row][col] = maxSize;
			}

			for (
				int s = (square + 1);
				s <= maxSquare;
				s++)
			{
				// heights larger that 'square' will not be available
				width2height[s] = 0;
			}
		}
	}

	// find the largest rectangle
	maxSize = 0;
	rectWidth = 0;
	rectHeight = 0;

	int rectRow = 0;
	int rectCol = 0;

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			size = sizes[row][col];

			if (size > maxSize)
			{
				maxSize = size;
				rectRow = row;
				rectCol = col;
				rectWidth = widths[row][col];
				rectHeight = heights[row][col];
			}
		}
	}

	return Rect(rectCol, rectRow, rectWidth, rectHeight);
}

int GetSize(int width, int height)
{
	return (width * height);
}

Rect findMinRect(const Mat1b& src)
{
	Mat1f W(src.rows, src.cols, float(0));
	Mat1f H(src.rows, src.cols, float(0));

	Rect maxRect(0, 0, 0, 0);
	float maxArea = 0.f;

	for (int r = 0; r < src.rows; ++r)
	{
		for (int c = 0; c < src.cols; ++c)
		{
			if (src(r, c) == 0)
			{
				H(r, c) = 1.f + ((r>0) ? H(r - 1, c) : 0);
				W(r, c) = 1.f + ((c>0) ? W(r, c - 1) : 0);
			}

			float minw = W(r, c);
			for (int h = 0; h < H(r, c); ++h)
			{
				minw = min(minw, W(r - h, c));
				float area = (h + 1) * minw;
				if (area > maxArea)
				{
					maxArea = area;
					maxRect = Rect(Point(c - minw + 1, r - h), Point(c + 1, r + 1));
				}
			}
		}
	}

	return maxRect;
}

Mat concatenateMatrix(Mat first, Mat second) {

	Mat mul1 = Mat::eye(3, 3, CV_64F);
	Mat mul2 = Mat::eye(3, 3, CV_64F);
	Mat x_;
	Mat temp_inv_;
	Mat mul_r;
	first.convertTo(temp_inv_, CV_64F);
	second.convertTo(x_, CV_64F);

	temp_inv_.row(0).copyTo(mul1.row(0));
	temp_inv_.row(1).copyTo(mul1.row(1));

	x_.row(1).copyTo(mul2.row(1));
	x_.row(0).copyTo(mul2.row(0));

	try {
		mul_r = mul1*mul2;
	}
	catch (Exception& e) {
		const char* err_msg = e.what();
		cout << err_msg;
	}

	mul1.release();
	mul2.release();
	temp_inv_.release();

	Mat b = mul_r.rowRange(1, mul_r.rows - 1).clone();

	return b;
}