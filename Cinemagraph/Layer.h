#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

enum class LayerType {
	VIDEO,
	STILL,
	NONE
};

class Layer
{
public:
	Layer();
	~Layer();

	bool LoadVideo(std::string path);
	bool LoadImage(std::string path);
	int FrameCount();
	void RenderFrame(int pos, cv::Mat &frame, cv::Mat &overlay);

private:
	cv::VideoCapture *video_capture = NULL;
	cv::Mat still;
	int video_capture_frame_count = -1;
	LayerType layer_type = LayerType::NONE;
};

