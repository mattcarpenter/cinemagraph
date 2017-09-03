#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include "Layer.h"
#include "Mask.h"
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
	void AddMask(Mask* mask);
	int GetWidth();
	int GetHeight();
	void DeleteLayer(ILayer* layer);

	Layer* GetVideoLayer();
	Layer* GetStillLayer();

private:

	void UpdateMask();

	Layer *video_layer;
	Layer *still_layer;

	cv::Mat video_frame;

	// Pre-allocated space for masks and overlays
	cv::Mat mask;
	cv::Mat mask_highlight;

	int start_frame = 0;
	int end_frame = -1;
	bool is_playing = false;

	std::vector<Mask*> masks;

	GpuMat gpu_frame;
	GpuMat gpu_still;
	GpuMat gpu_mask;
	GpuMat gpu_mask_highlight;
	GpuMat gpu_frame_mask;
	GpuMat gpu_still_with_alpha;
	GpuMat gpu_frame_with_alpha;
	GpuMat gpu_dest_with_alpha;
	GpuMat gpu_dest;
};

