#include "CaptureFrame.h"
#include "qdebug.h"

CaptureFrame::CaptureFrame(cv::Mat f, int num)
{
	f.copyTo(frame);
	frame_number = num;
}

CaptureFrame::~CaptureFrame()
{
}

void CaptureFrame::CopyFrameTo(cv::Mat &f)
{
	this->frame.copyTo(f);
}

cv::Mat CaptureFrame::GetFrame()
{
	return this->frame;
}

int CaptureFrame::GetFrameNumber()
{
	return frame_number;
}
