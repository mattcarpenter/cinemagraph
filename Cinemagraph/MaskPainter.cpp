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
	brush_size = max(1, brush_size + delta);
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
	float opaque = brush_opacity;
	float hardness = brush_hardness / 100.0f;
	float aspect_ratio = 1;

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
				opa = dd + 1 - (dd / hardness); //outer
			else
				opa = hardness / (1 - hardness)*(1 - dd); //inner
			
			opa = opa * (1.0f + (dd/2.0f));
			float pixel_opacity = opa * opaque;
			pixel_opacity = min(pixel_opacity, opaque);

			if (pixel_opacity > opaque - 4) {
				pixel_opacity = opaque;
			}

			dab.at<uchar>(Point(px, py)) = pixel_opacity;

		}
	}

	// draw bounding box around dab (useful for position/size debugging)
	//rectangle(dab, Rect(0, 0, dab.cols - 1, dab.rows - 1), Scalar(255), 1);
}

void MaskPainter::DrawBrush(cv::Mat target, int x, int y)
{
	if (x < 0 || y < 0 || x > target.cols || y > target.rows)
		return;

	Rect dest_rect = Rect(x - (brush_size / 2), y - (brush_size / 2), brush_size, brush_size);
	
	int left_clip = dest_rect.x < 0 ? -1 * dest_rect.x : 0;
	int top_clip = dest_rect.y < 0 ? -1 * dest_rect.y : 0;

	int right_clip = (dest_rect.x + dest_rect.width) > target.cols ? (dest_rect.x + dest_rect.width) - target.cols : 0;
	int bottom_clip = (dest_rect.y + dest_rect.height) > target.rows ? (dest_rect.y + dest_rect.height) - target.rows : 0;

	Rect dest_roi = Rect(dest_rect.x + left_clip, dest_rect.y + top_clip, dest_rect.width - right_clip - left_clip, dest_rect.height - bottom_clip - top_clip);

	if (mode == PaintMode::PAINT_BRUSH)
		target(dest_roi) = cv::max(dab(Rect(left_clip, top_clip, brush_size - right_clip - left_clip, brush_size - bottom_clip - top_clip)), target(dest_roi));
	else
		target(dest_roi) = cv::min(Scalar(255) - dab(Rect(left_clip, top_clip, brush_size - right_clip - left_clip, brush_size - bottom_clip - top_clip)), target(dest_roi));
}
