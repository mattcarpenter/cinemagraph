#include "VideoWorker.h"

Mat blend_multiply(const Mat& level1, const Mat& level2, uchar opacity);

VideoWorker::VideoWorker(Video* video)
{
	this->video = video;
}

void VideoWorker::GeneratePreview()
{
	// Grab first frame for the preview
	Mat first_frame;
	if (video->FrameAt(0, first_frame)) {
		first_frame.copyTo(current_frame);
		emit frame(first_frame);
	}
}

void VideoWorker::AddMask(cv::Point a, cv::Point b)
{
	video->AddMask(a, b);
	emit frame(CurrentFrameWithMask());
}

cv::Mat VideoWorker::CurrentFrameWithMask()
{
	Mat mask = video->Mask();
	Mat out;
	current_frame.copyTo(out);

	for (int x = 0; x < mask.cols; x++)
	{
		for (int y = 0; y < mask.rows; y++)
		{
			if (mask.at<uchar>(Point(x, y)) == 255)
			{
				out.at<Vec3b>(Point(x, y))[2] += 50;
			}
		}
	}

	return out;
}

void VideoWorker::DetectFeatures()
{

}

void VideoWorker::Stabilize()
{
	this->video->Stabilize([&](cv::Mat f) {
		f.copyTo(current_frame);
		emit frame(f);
	});
}

Mat blend_multiply(const Mat& level1, const Mat& level2, uchar opacity)
{
	CV_Assert(level1.size() == level2.size());
	CV_Assert(level1.type() == level2.type());
	CV_Assert(level1.channels() == level2.channels());

	// Get 4 channel float images
	Mat4f src1, src2;

	if (level1.channels() == 3)
	{
		Mat4b tmp1, tmp2;
		cvtColor(level1, tmp1, COLOR_BGR2BGRA);
		cvtColor(level2, tmp2, COLOR_BGR2BGRA);
		tmp1.convertTo(src1, CV_32F, 1. / 255.);
		tmp2.convertTo(src2, CV_32F, 1. / 255.);
	}
	else
	{
		level1.convertTo(src1, CV_32F, 1. / 255.);
		level2.convertTo(src2, CV_32F, 1. / 255.);
	}

	Mat4f dst(src1.rows, src1.cols, Vec4f(0., 0., 0., 0.));

	// Loop on every pixel

	float fopacity = opacity / 255.f;
	float comp_alpha, new_alpha;

	for (int r = 0; r < src1.rows; ++r)
	{
		for (int c = 0; c < src2.cols; ++c)
		{
			const Vec4f& v1 = src1(r, c);
			const Vec4f& v2 = src2(r, c);
			Vec4f& out = dst(r, c);

			comp_alpha = min(v1[3], v2[3]) * fopacity;
			new_alpha = v1[3] + (1.f - v1[3]) * comp_alpha;

			if ((comp_alpha > 0.) && (new_alpha > 0.))
			{
				float ratio = comp_alpha / new_alpha;

				out[0] = max(0.f, min(v1[0] * v2[0], 1.f)) * ratio + (v1[0] * (1.f - ratio));
				out[1] = max(0.f, min(v1[1] * v2[1], 1.f)) * ratio + (v1[1] * (1.f - ratio));
				out[2] = max(0.f, min(v1[2] * v2[2], 1.f)) * ratio + (v1[2] * (1.f - ratio));
			}
			else
			{
				out[0] = v1[0];
				out[1] = v1[1];
				out[2] = v1[2];
			}

			out[3] = v1[3];

		}
	}

	Mat3b dst3b;
	Mat4b dst4b;
	dst.convertTo(dst4b, CV_8U, 255.);
	cvtColor(dst4b, dst3b, COLOR_BGRA2BGR);

	return dst3b;
}