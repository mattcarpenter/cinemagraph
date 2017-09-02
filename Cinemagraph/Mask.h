#pragma once
#include "ILayer.h"
#include <opencv2/opencv.hpp>

class Mask :
	public ILayer
{
public:
	Mask(int width, int height);
	~Mask();

	bool GetVisible();
	void SetVisible(bool v);
	LayerType GetType();

private:
	bool visible = true;
	cv::Mat mask;
};

