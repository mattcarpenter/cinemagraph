#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <mutex>
#include "Semaphore.h"
#include "CaptureFrame.h"
#include "ILayer.h"

enum class LayerType {
	VIDEO,
	STILL,
	NONE
};

class Layer : public ILayer
{
public:
	Layer();
	~Layer();

	bool LoadVideo(std::string path, function<void(cv::Mat)> thumb_callback);
	bool LoadImage(std::string path);
	
	int GetFrameCount();
	void SetStartFrame(int sf);
	void SetEndFrame(int ef);
	void SetPlaying(bool playing);
	void Seek(int pos);
	
	int RenderNextFrame(cv::Mat &frame);
private:
	void CaptureLoop();

	LayerType layer_type = LayerType::NONE;
	
	cv::Mat still;
	cv::VideoCapture *video_capture = NULL;
	
	std::thread capture_thread;
	std::queue<CaptureFrame*> capture_queue;
	Semaphore *capture_sem;

	const int capture_queue_max_length = 10;
	const int thumbnail_height = 50;
	const int thumb_count = 20;
	const int thumb_height = 50;

	int video_capture_frame_count = -1;
	int current_frame_number = 0;
	int start_frame = 0;
	int end_frame = -1;
	bool is_playing = false;

	int seek_to_frame = -1;
};

