#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include "Layer.h"

using namespace std;

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

	Layer* GetVideoLayer();
	Layer* GetStillLayer();

private:
	Layer *video_layer;
	Layer *still_layer;

	cv::Mat video_frame;

	int start_frame = 0;
	int end_frame = -1;
	bool is_playing = false;
};

