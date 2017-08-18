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

void CaptureFrame::GetFrame(cv::Mat &f)
{
	this->frame.copyTo(f);
}

int CaptureFrame::GetFrameNumber()
{
	return frame_number;
}
