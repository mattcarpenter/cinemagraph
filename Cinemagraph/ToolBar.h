#pragma once

#include <QToolBar>

enum class PaintMode {
	PAINT_BRUSH,
	ERASER
};

class ToolBar : public QToolBar
{
	Q_OBJECT

public:
	ToolBar(QWidget *parent);
	~ToolBar();

private slots:
	void paint_brush_button_clicked();
	void eraser_button_clicked();

signals:
	void PaintBrushOn();
	void PaintBrushOff();
	void EraserOn();
	void EraserOff();

private:
	bool paint_brush_state;
	bool eraser_state;
};
