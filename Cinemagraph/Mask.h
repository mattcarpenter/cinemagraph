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
	bool GetHighlighted();
	void SetHighlighted(bool h);
	std::string GetName();
	cv::Mat GetMat();
	LayerType GetType();
	cv::Mat GetPreview();
	cv::Mat GetCommitted();
	bool IsEditing();
	void SetIsEditing(bool e);

private:
	std::string name;
	
	bool visible = true;
	bool highlighted = false;

	cv::Mat mask;
	cv::Mat blank;
	cv::Mat preview;
	cv::Mat committed;

	bool is_editing = false;
};
