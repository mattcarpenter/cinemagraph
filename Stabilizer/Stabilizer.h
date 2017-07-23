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
	static void Stabilizer::Stabilize(VideoCapture* cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback, bool just_detect);
private:
	static vector<cv::Mat> Stabilizer::StabilizePass1(VideoCapture *cap, cv::Mat mask, ::function<void(cv::Mat frame)> callback, bool just_detect);
	static vector<cv::Mat> Stabilizer::StabilizePass2(VideoCapture *cap, ::function<void(cv::Mat frame)> callback, vector<Mat> transforms, cv::Mat &mask_accumulated);
	static void StabilizePass3(VideoCapture *cap, cv::Mat mask, std::function<void(cv::Mat frame)> callback, vector<cv::Mat> transforms, vector<cv::Mat> transforms_pass_2);
	static Rect Stabilizer::LargestInscribedRect(cv::Mat mask);
};

