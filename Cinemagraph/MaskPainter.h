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
private:
	Composition *composition;
	Mask *target = NULL;
	bool committing = false;

	bool CheckPreconditions();
	bool CheckPreconditions(bool reset);
	void Reset(int w, int h);
	PaintMode mode = PaintMode::PAINT_BRUSH;
	int brush_size = 50;
	int last_x = 0;
	int last_y = 0;
};
