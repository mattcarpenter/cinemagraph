#include "VideoWorker.h"

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

void VideoWorker::ResetMask()
{
	video->ResetMask();
	emit frame(current_frame);
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
				uchar val = out.at<Vec3b>(Point(x, y))[2];
				if (val + 50 > 255)
				{
					val = 255;
				}
				else
				{
					val += 50;
				}
				out.at<Vec3b>(Point(x, y))[2] = val;
			}
		}
	}

	return out;
}

void VideoWorker::DetectFeatures()
{
	this->video->DetectFeatures([&](cv::Mat f) {
		f.copyTo(current_frame);
		emit frame(f);
	});
}

void VideoWorker::Stabilize()
{
	this->video->Stabilize([&](cv::Mat f) {
		f.copyTo(current_frame);
		emit frame(f);
	});

	// Reset
	video->FrameAt(0, current_frame);
	emit frame(CurrentFrameWithMask());
}
