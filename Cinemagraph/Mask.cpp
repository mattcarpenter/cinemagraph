#include "Mask.h"

using namespace std;
using namespace cv;

Mask::Mask(int width, int height, std::string mask_name)
{
	mask = Mat(height, width, CV_8UC1, Scalar(0));
	name = mask_name;

	// TODO - Remove
	mask = Mat(height, width, CV_8UC1, Scalar(0));
	rectangle(mask, Rect(20, 20, 200, 200), Scalar(255), -1);
	for (int z = 220; z < 220 + 255; z++) {
		line(mask, Point(z, 20), Point(z, 220), Scalar(255 - (z - 220)));
	}
}

Mask::~Mask()
{
}

void Mask::SetVisible(bool v)
{
	visible = v;
}

bool Mask::GetVisible()
{
	return visible;
}

LayerType Mask::GetType()
{
	return LayerType::MASK;
}

cv::Mat Mask::GetMat()
{
	return mask;
}
