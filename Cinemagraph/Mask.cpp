#include "Mask.h"

using namespace std;
using namespace cv;

Mask::Mask(int width, int height, std::string mask_name)
{
	mask = Mat(height, width, CV_8UC1, Scalar(0));
	mask.copyTo(blank);
	mask.copyTo(preview);
	mask.copyTo(committed);
	name = mask_name;

	mask = Mat(height, width, CV_8UC1, Scalar(0));
}

std::string Mask::GetName()
{
	return name;
}

Mask::~Mask()
{
}

void Mask::SetPaintMode(PaintMode m)
{
	paint_mode = m;
}

PaintMode Mask::GetPaintMode()
{
	return paint_mode;
}

void Mask::SetVisible(bool v)
{
	visible = v;
}

bool Mask::GetVisible()
{
	return visible;
}

void Mask::SetHighlighted(bool h)
{
	highlighted = h;
}

bool Mask::GetHighlighted()
{
	return highlighted;
}

LayerType Mask::GetType()
{
	return LayerType::MASK;
}

cv::Mat Mask::GetMat()
{
	return visible ? mask : blank;
}

cv::Mat Mask::GetPreview()
{
	return preview;
}

cv::Mat Mask::GetCommitted()
{
	return committed;
}

bool Mask::IsEditing()
{
	return is_editing;
}

void Mask::SetIsEditing(bool e)
{
	is_editing = e;
}
