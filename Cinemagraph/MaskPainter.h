#pragma once
#include "Composition.h"
#include "Mask.h"
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
private:
	Composition *composition;
	Mask *target = NULL;
	bool committing = false;

	bool CheckPreconditions();
	bool CheckPreconditions(bool reset);
	void Reset(int w, int h);
};
