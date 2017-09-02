#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include "Layer.h"
#include <opencv2/cudaimgproc.hpp>
using namespace std;
using namespace cv::cuda;

class Composition
{
public:
	Composition();
	~Composition();
	
	bool LoadVideo(string path, function<void(cv::Mat)> thumb_callback);
	bool LoadStill(string path);
	int Render(cv::Mat &target);

	int GetFrameCount();
	void SetStartFrame(int sf);
	int GetStartFrame();
	void SetEndFrame(int ef);
	int GetEndFrame();
	void SetPlaying(bool playing);
	void Seek(int pos);
	void SetCurrentVideoFrameAsStill();

	Layer* GetVideoLayer();
	Layer* GetStillLayer();

private:
	Layer *video_layer;
	Layer *still_layer;

	cv::Mat video_frame;

	cv::Mat mask;

	int start_frame = 0;
	int end_frame = -1;
	bool is_playing = false;

	GpuMat gpu_frame;
	GpuMat gpu_still;
	GpuMat gpu_mask;
	GpuMat gpu_frame_with_alpha;
	GpuMat gpu_dest_with_alpha;
	GpuMat gpu_dest;
};

