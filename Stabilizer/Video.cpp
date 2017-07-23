#define _CRT_SECURE_NO_WARNINGS

#include "Video.h"
#include "Stabilizer.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <functional>
#include <thread>

using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;

Video::Video(char* path)
{
	this->cap = new VideoCapture(path);
	ResetMask();
}

void Video::ResetMask()
{
	width = this->cap->get(CAP_PROP_FRAME_WIDTH);
	height = this->cap->get(CAP_PROP_FRAME_HEIGHT);
	mask = Mat(Size(width, height), CV_8UC1, Scalar(0));
	mask.setTo(Scalar::all(255));
	mask_dirty = false;
}

bool Video::FrameAt(int frame_number, cv::Mat &dest)
{
	Mat frame;
	
	if (!cap)
		return false;
	
	cap->set(CAP_PROP_POS_FRAMES, frame_number);
	
	if (!cap->read(frame))
		return false;
	
	frame.copyTo(dest);

	return true;
}

void Video::AddMask(cv::Point a, cv::Point b)
{
	if (!mask_dirty)
	{
		mask_dirty = true;
		mask.setTo(Scalar::all(0));
	}
	rectangle(mask, Rect(a, b), Scalar(255, 255, 255), -1);
}

cv::Mat Video::Mask()
{
	return mask;
}

int Video::Width()
{
	return width;
}

int Video::Height()
{
	return height;
}

void Video::Stabilize(std::function<void(cv::Mat frame)> callback)
{
	Stabilizer::Stabilize(this->cap, mask, callback, false);
}

void Video::DetectFeatures(std::function<void(cv::Mat frame)> callback)
{
	Stabilizer::Stabilize(this->cap, mask, callback, true);
}

Video::~Video()
{
}
