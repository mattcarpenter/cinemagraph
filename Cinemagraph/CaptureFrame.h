#pragma once
#include <opencv2/opencv.hpp>

class CaptureFrame
{
public:
	CaptureFrame(cv::Mat f, int num);
	~CaptureFrame();
	void CopyFrameTo(cv::Mat &f);
	cv::Mat GetFrame();
	int GetFrameNumber();
private:
	cv::Mat frame;
	int frame_number = 0;
};

