#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <functional>

using namespace std;
using namespace cv;

class Video
{
public:
	Video(char* path);
	~Video();

	void Stabilize(std::function<void(cv::Mat frame)> callback);
	void DetectFeatures(std::function<void(cv::Mat frame)> callback);
	int Width();
	int Height();
	cv::Mat Mask();
	bool FrameAt(int frame_number, cv::Mat &dest);
	void AddMask(cv::Point a, cv::Point b);
	void ResetMask();
private:
	VideoCapture* cap;
	int width;
	int height;
	cv::Mat mask;
	bool mask_dirty;
};

