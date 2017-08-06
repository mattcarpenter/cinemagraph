#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <mutex>
#include "Semaphore.h"

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
	//void RenderFrame(int pos, cv::Mat &frame, cv::Mat &overlay);
	void PopFrame(cv::Mat &frame);
private:
	void CaptureLoop();

	cv::VideoCapture *video_capture = NULL;
	cv::Mat still;

	int video_capture_frame_count = -1;
	int current_frame_number = 0;
	
	LayerType layer_type = LayerType::NONE;

	std::thread capture_thread;
	std::queue<cv::Mat> capture_queue;
	Semaphore *capture_sem;
	int capture_queue_max_length = 10;
};

