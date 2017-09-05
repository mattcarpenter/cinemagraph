#include "MaskPainter.h"

using namespace std;
using namespace cv;

MaskPainter::MaskPainter(Composition *comp)
{
	composition = comp;
}

MaskPainter::~MaskPainter()
{
}

void MaskPainter::MouseMove(int x, int y)
{
	if (!CheckPreconditions())
		return;

	int width = target->GetMat().cols;
	int height = target->GetMat().rows;

	// TODO - Eraser

	if (committing)
	{
		rectangle(target->GetCommitted(), Rect(x-50, y-50, 100, 100), Scalar(255), -1);
	}
	else
	{
		target->GetPreview() = Mat::zeros(height, width, CV_8UC1);
		rectangle(target->GetPreview(), Rect(x-50, y-50, 100, 100), Scalar(255), -1);
	}
}

void MaskPainter::MouseDown(int x, int y)
{
	if (!CheckPreconditions(true))
		return;

	committing = true;
}

void MaskPainter::MouseUp(int x, int y)
{
	if (!CheckPreconditions())
		return;

	committing = false;

	// Copy committed mask to the target
	cv::bitwise_or(target->GetMat(), target->GetCommitted(), target->GetMat());
}

bool MaskPainter::CheckPreconditions()
{
	return CheckPreconditions(false);
}

/**
 * Returns false if we're not currently in a state where we can draw to a mask.
 *
 * @param {bool} reset pass true to clear out the preview and committed Mats
 * @returns {bool}
 */
bool MaskPainter::CheckPreconditions(bool reset)
{
	// Must have a target mask
	if (target == NULL)
		return false;

	int target_width = target->GetMat().cols;
	int target_height = target->GetMat().rows;

	if (reset)
		Reset(target_width, target_height);

	// Must have dimensions
	if (!target_width || !target_height)
		return false;

	if (target->GetPreview().cols != target_width || target->GetPreview().rows != target_height
		|| target->GetCommitted().cols != target_width || target->GetCommitted().rows != target_height)
	{
		Reset(target_width, target_height);
	}

	return true;
}

void MaskPainter::Reset(int w, int h)
{
	target->GetPreview() = Mat::zeros(w, h, CV_8UC1);
	target->GetPreview().copyTo(target->GetCommitted());
}

void MaskPainter::AttachLayer(Mask* m)
{
	target = m;
	target->SetIsEditing(true);
}

void MaskPainter::DetachLayer()
{
	if (target == NULL)
		return;

	Reset(target->GetMat().cols, target->GetMat().rows);
	target->SetIsEditing(false);
	target = NULL;
}

Mask* MaskPainter::GetTarget()
{
	return target;
}
