#pragma once
#include "Composition.h"
#include "Mask.h"
#include "ToolBar.h"
#include <opencv2/opencv.hpp>

class MaskPainter
{
public:
	MaskPainter(Composition *comp);
	~MaskPainter();
	void MouseMove(int x, int y);
	void MouseDown(int x, int y);
	void MouseUp(int x, int y);
	void AttachLayer(Mask* m);
	void DetachLayer();
	Mask* GetTarget();
	void PaintBrushOn();
	void PaintBrushOff();
	void EraserOn();
	void EraserOff();
	PaintMode GetPaintMode();
	void ChangeBrushSize(int delta);
	void ChangeBrushHardness(int hardness);

private:
	bool CheckPreconditions();
	bool CheckPreconditions(bool reset);
	void Reset(int w, int h);
	void DrawBrush(cv::Mat target, int x, int y, cv::Scalar color);

	Composition *composition;
	Mask *target = NULL;
	PaintMode mode = PaintMode::PAINT_BRUSH;
	
	bool committing = false;
	int brush_size = 50;
	int last_x = 0;
	int last_y = 0;
	int brush_hardness = 80;
};
