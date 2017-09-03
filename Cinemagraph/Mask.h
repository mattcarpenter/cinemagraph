#pragma once
#include "ILayer.h"
#include <opencv2/opencv.hpp>

class Mask :
	public ILayer
{
public:
	Mask(int width, int height, std::string mask_name);
	~Mask();

	bool GetVisible();
	void SetVisible(bool v);
	LayerType GetType();

private:
	std::string name;
	bool visible = true;
	cv::Mat mask;

};

