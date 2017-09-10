#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent)
	: QToolBar(parent)
{
	// Set up toolbar
	QIcon pencil_icon = QIcon(":/Cinemagraph/Resources/pencil.png");
	QIcon eraser_icon = QIcon(":/Cinemagraph/Resources/eraser.png");
	QAction *paint_action = new QAction(pencil_icon, "Paint Brush", this);
	QAction *erase_action = new QAction(eraser_icon, "Eraser", this);
	connect(paint_action, SIGNAL(triggered()), this, SLOT(paint_brush_button_clicked()));
	connect(erase_action, SIGNAL(triggered()), this, SLOT(eraser_button_clicked()));
	this->addAction(paint_action);
	this->addAction(erase_action);
}

ToolBar::~ToolBar()
{
}

void ToolBar::paint_brush_button_clicked()
{
	emit EraserOff();
	emit PaintBrushOn();
}

void ToolBar::eraser_button_clicked()
{
	emit PaintBrushOff();
	emit EraserOn();
}
