#pragma once
#include <opencv2/opencv.hpp>
#include <functional>

using namespace std;
using namespace cv;

class Stabilizer
{
public:
	Stabilizer();
	~Stabilizer();
	static void Stabilizer::Stabilize(VideoCapture* cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback);
};

