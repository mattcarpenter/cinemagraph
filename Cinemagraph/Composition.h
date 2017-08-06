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
	
	bool LoadVideo(string path);
	bool LoadStill(string path);
	void Render(cv::Mat &target);

	int GetFrameCount();
	void SetStartFrame(int sf);
	void SetEndFrame(int ef);
	void SetPlaying(bool playing);

private:
	Layer *video_layer;
	Layer *still_layer;
	int start_frame = 0;
	int end_frame = -1;
	bool is_playing = false;
};

