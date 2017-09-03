#include "Mask.h"

using namespace std;
using namespace cv;

Mask::Mask(int width, int height, std::string mask_name)
{
	mask = Mat(height, width, CV_8UC1, Scalar(0));
	name = mask_name;
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
