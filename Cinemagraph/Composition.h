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
	bool Render(int frame_number, cv::Mat &target);

private:
	Layer *video_layer;
	Layer *still_layer;
};

