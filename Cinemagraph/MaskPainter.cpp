#include "MaskPainter.h"
#include <qdebug.h>

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
	last_x = x;
	last_y = y;

	if (!CheckPreconditions())
		return;

	int width = target->GetMat().cols;
	int height = target->GetMat().rows;

	if (committing)
	{
		DrawBrush(target->GetCommitted(), x, y);
	}
	else
	{
		target->GetPreview() = Scalar(mode == PaintMode::ERASER ? 255 : 0);
		DrawBrush(target->GetPreview(), x, y);
	}
}

PaintMode MaskPainter::GetPaintMode()
{
	return mode;
}

void MaskPainter::MouseDown(int x, int y)
{
	last_x = x;
	last_y = y;

	if (!CheckPreconditions(true))
		return;

	committing = true;
}

void MaskPainter::MouseUp(int x, int y)
{
	last_x = x;
	last_y = y;

	if (!CheckPreconditions())
		return;

	committing = false;

	// Copy committed mask to the target
	if (mode == PaintMode::PAINT_BRUSH)
	{
		cv::bitwise_or(target->GetMat(), target->GetCommitted(), target->GetMat());
	}
	else
	{
		cv::bitwise_and(target->GetMat(), target->GetCommitted(), target->GetMat());
	}
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

	target->SetPaintMode(mode);

	return true;
}

void MaskPainter::Reset(int w, int h)
{
	target->GetPreview() = Mat::zeros(w, h, CV_8UC1);
	if (mode == PaintMode::ERASER)
		target->GetPreview() = Scalar(255);
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

void MaskPainter::PaintBrushOn()
{
	mode = PaintMode::PAINT_BRUSH;

	if (target != NULL)
	{
		Reset(target->GetMat().cols, target->GetMat().rows);
		target->SetPaintMode(mode);
	}
}

void MaskPainter::PaintBrushOff()
{

}

void MaskPainter::EraserOn()
{
	mode = PaintMode::ERASER;
	
	if (target != NULL)
	{
		target->SetPaintMode(mode);
		Reset(target->GetMat().cols, target->GetMat().rows);
	}
}

void MaskPainter::EraserOff()
{

}

void MaskPainter::ChangeBrushSize(int delta)
{
	brush_size += delta;
	MouseMove(last_x, last_y);
}

void MaskPainter::SetBrushHardness(int hardness)
{
	brush_hardness = hardness;
	MouseMove(last_x, last_y);
}

void MaskPainter::SetBrushOpacity(int opacity)
{
	brush_opacity = opacity;
	MouseMove(last_x, last_y);
}

void MaskPainter::DrawBrush(cv::Mat target, int x, int y)
{
	Scalar color = (mode == PaintMode::PAINT_BRUSH ? brush_opacity : 255 - brush_opacity);
	rectangle(target, Rect(x - (brush_size / 2), y - (brush_size / 2), brush_size, brush_size), color, -1);
}
