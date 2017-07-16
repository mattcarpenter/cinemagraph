#define _CRT_SECURE_NO_WARNINGS

#include "Stabilizer.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <functional>

using namespace std;
using namespace cv;

Stabilizer::Stabilizer()
{
}


Stabilizer::~Stabilizer()
{
}

void Stabilizer::Stabilize(VideoCapture *cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback)
{
	Mat current, current_gray;
	Mat previous, previous_gray;
	Mat still, still_gray;
	Mat warped_mask;

	mask.copyTo(warped_mask);

	double offset_x, offset_y;

	Mat last_T;

	still = imread("C:\\Users\\mattc\\Desktop\\temp\\scaled.png", 1);

	if (!cap->read(previous))
		return;

	cvtColor(previous, previous_gray, COLOR_BGR2GRAY);
	cvtColor(still, still_gray, COLOR_BGR2GRAY);

	VideoWriter outputVideo;

	Size S = Size((int)cap->get(CV_CAP_PROP_FRAME_WIDTH), (int)cap->get(CV_CAP_PROP_FRAME_HEIGHT));
	//CV_FOURCC('P', 'I', 'M', '1')
	outputVideo.open("C:\\Users\\mattc\\Desktop\\temp\\out.avi", -1, cap->get(CV_CAP_PROP_FPS), S, true);

	//Mat mask(previous.size(), CV_8UC1, Scalar(0));
	//mask.setTo(Scalar::all(255));
	//Scalar color(255, 255, 255);
	//rectangle(mask, Point(300, 170), Point(600, 267), color, CV_FILLED, 0); //93,427  260,460
	//rectangle(mask, Point(93, 427), Point(260, 460), color, CV_FILLED, 0);

	//rectangle(mask, Point(1684, 788), Point(2323, 1000), color, CV_FILLED, 0); //93,427  260,460
	//rectangle(mask, Point(350, 1630), Point(930, 1740), color, CV_FILLED, 0);
	//rectangle(mask, Point(0, 0), Point(400, 150), color, CV_FILLED, 0);
	//imshow("mask", mask);
	while (cap->read(current))
	{
		cvtColor(current, current_gray, COLOR_BGR2GRAY);

		// vector from prev to cur
		vector <Point2f> previous_corner, current_corner;
		vector <Point2f> previous_corner2, current_corner2;
		vector <uchar> status;
		vector <float> err;

		goodFeaturesToTrack(previous_gray, previous_corner, 25, 0.01, 10, warped_mask, 3, true);
		calcOpticalFlowPyrLK(previous_gray, current_gray, previous_corner, current_corner, status, err);
		
		// weed out bad matches
		for (size_t i = 0; i < status.size(); i++) {
			if (status[i]) {
				previous_corner2.push_back(previous_corner[i]);
				current_corner2.push_back(current_corner[i]);
			}
		}

		// translation + rotation only
		Mat T = estimateRigidTransform(previous_corner2, current_corner2, false); // false = rigid transform, no scaling/shearing
																				  // in rare cases no transform is found. We'll just use the last known good transform.
		if (T.data == NULL) {
			cout << "T.data null" << endl;
			last_T.copyTo(T);
		}

		T.copyTo(last_T);

		// decompose T
		double dx = T.at<double>(0, 2);
		double dy = T.at<double>(1, 2);
		double da = atan2(T.at<double>(1, 0), T.at<double>(0, 0));

		cout << "dx: " << dx << " dy: " << dy << endl;
		
		Mat out, out_dest;
		current.copyTo(out);

		for (int i = 0; i < current_corner2.size(); i++)
		{
			Point p = current_corner2[i];
			line(out, Point(p.x - 7, p.y), Point(p.x + 7, p.y), Scalar(100, 0, 255), 1);
			line(out, Point(p.x, p.y - 7), Point(p.x, p.y + 7), Scalar(100, 0, 255), 1);
		}

		Mat dest;
		Mat temp_mask;
		warpAffine(current, dest, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP); //INTER_LANCZOS4
		warpAffine(out, out_dest, T, current.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP); //INTER_LANCZOS4
		warpAffine(mask, temp_mask, T, current.size());// WARP_INVERSE_MAP);
		temp_mask.copyTo(warped_mask);
		outputVideo.write(dest);
		callback(out_dest);
		current.copyTo(previous);
	}
}
