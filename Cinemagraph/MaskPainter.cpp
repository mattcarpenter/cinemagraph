#include "MaskPainter.h"
#include <qdebug.h>
#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */
using namespace std;
using namespace cv;

MaskPainter::MaskPainter(Composition *comp)
{
	composition = comp;
	GenerateDab();
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
		target->GetMat() = cv::max(target->GetMat(), target->GetCommitted());
	}
	else
	{
		// TODO - is bitwise_and right?
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
		Reset();

	// Must have dimensions
	if (!target_width || !target_height)
		return false;

	if (target->GetPreview().cols != target_width || target->GetPreview().rows != target_height
		|| target->GetCommitted().cols != target_width || target->GetCommitted().rows != target_height)
	{
		Reset();
	}

	target->SetPaintMode(mode);

	return true;
}

void MaskPainter::Reset()
{
	if (mode == PaintMode::ERASER)
	{
		target->GetPreview() = Scalar(255);
		target->GetCommitted() = Scalar(255);
	}
	else if (mode == PaintMode::PAINT_BRUSH)
	{
		target->GetPreview() = Scalar(0);
		target->GetCommitted() = Scalar(0);
	}
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

	Reset();
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
		target->SetPaintMode(mode);
		Reset();
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
		Reset();
	}
}

void MaskPainter::EraserOff()
{

}

void MaskPainter::ChangeBrushSize(int delta)
{
	brush_size += delta;
	GenerateDab();
	MouseMove(last_x, last_y);
}

void MaskPainter::SetBrushHardness(int hardness)
{
	brush_hardness = hardness;
	GenerateDab();
	MouseMove(last_x, last_y);
}

void MaskPainter::SetBrushOpacity(int opacity)
{
	brush_opacity = opacity;
	GenerateDab();
	MouseMove(last_x, last_y);
}

void MaskPainter::GenerateDab()
{
	dab = Mat::zeros(brush_size, brush_size, CV_8UC1);

	// TODO - Erase versus Paint
	// TODO - Opacity and Hardness

	int x = brush_size / 2;
	int y = brush_size / 2;
	float angle = 90;
	float cs = cos(angle / 360 * 2 * M_PI);
	float sn = sin(angle / 360 * 2 * M_PI);
	float opaque = 255;
	float hardness = 0.7;
	float aspect_ratio = 1;
	// Enumerate each pixel in the a bounding box
	int r = brush_size / 2;
	for (int px = x - r; px < x + r; px++)
	{
		for (int py = y - r; py < y + r; py++)
		{
			float opa = 0;

			float dx = px - x;
			float dy = py - y;
			float dyr = (dy*cs - dx*sn)*aspect_ratio;
			float dxr = (dy*sn + dx*cs);
			float dd = (dyr*dyr + dxr*dxr) / (r*r);
			if (dd > 1)
				opa = 0;
			else if (dd < hardness)
				opa = dd + 1 - (dd / hardness);
			else
				opa = hardness / (1 - hardness)*(1 - dd);
			float pixel_opacity = opa * opaque;

			dab.at<uchar>(Point(px, py)) = pixel_opacity;

		}
	}

	// draw bounding box around dab (useful for position/size debugging)
	// rectangle(dab, Rect(0, 0, dab.cols - 1, dab.rows - 1), Scalar(255), 1);
}

void MaskPainter::DrawBrush(cv::Mat target, int x, int y)
{
	Rect roi_rect = Rect(
		max(0, x - (brush_size / 2)),
		max(0, y - (brush_size / 2)),
		brush_size,
		brush_size
	);

	if (roi_rect.width + roi_rect.x > target.cols)
		roi_rect.width = target.cols - roi_rect.x;

	if (roi_rect.height + roi_rect.y > target.rows)
		roi_rect.height = target.rows - roi_rect.y;

	target(roi_rect) = cv::max(dab(Rect(0, 0, roi_rect.width, roi_rect.height)), target(roi_rect));
}
