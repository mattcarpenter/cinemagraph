#include "VideoView.h"
#include <iostream>
#include "qapplication.h"
#include "qdebug.h"
#include "asmOpenCV.h"
#include "qevent.h"

using namespace cv;

VideoView::VideoView(QWidget *parent)
	: QLabel(parent)
{
}

VideoView::~VideoView()
{
}

void VideoView::SetVideoFrame(cv::Mat frame)
{
	frame.copyTo(current_frame);
	frame.copyTo(preview_buffer);
}

void VideoView::PaintVideoPreview()
{
	QPixmap pmFrame = ASM::cvMatToQPixmap(preview_buffer);
	int width = pmFrame.width();
	int height = pmFrame.height();
	this->setPixmap(pmFrame.scaled(width, height, Qt::KeepAspectRatio));
}

void VideoView::enterEvent(QEvent * event)
{
	QApplication::setOverrideCursor(Qt::CrossCursor);
}

void VideoView::leaveEvent(QEvent * event)
{
	QApplication::restoreOverrideCursor();
}

void VideoView::mouseReleaseEvent(QMouseEvent * event)
{
	marquee_end = event->pos();
	
	if (marquee_end.x() == marquee_start.x() && marquee_end.y() == marquee_start.y())
		return;
	
	current_frame.copyTo(preview_buffer);
	PaintVideoPreview();
	emit AddMask(MouseToVideoPoint(marquee_start), MouseToVideoPoint(marquee_end));
}

void VideoView::mousePressEvent(QMouseEvent * event)
{
	marquee_start = event->pos();
}

void VideoView::mouseMoveEvent(QMouseEvent * event)
{
	QPoint marquee_current = event->pos();
	QPoint marquee_start_video = MouseToVideoPoint(marquee_start);
	QPoint marquee_current_video = MouseToVideoPoint(marquee_current);
	
	// Start with the current video frame
	current_frame.copyTo(preview_buffer);

	// Draw marquee
	rectangle(preview_buffer, Rect(Point(marquee_start_video.x(), marquee_start_video.y()), Point(marquee_current_video.x(), marquee_current_video.y())), Scalar(0, 0, 255), 1);
	PaintVideoPreview();
}

void VideoView::SetMask(cv::Mat m)
{
	m.copyTo(mask);
}

QPoint VideoView::MouseToVideoPoint(QPoint mouse_point)
{
	if (current_frame.cols == 0 || current_frame.rows == 0)
		return QPoint(0, 0);

	// Calculate margins
	int margin_x = (this->width() - current_frame.cols) / 2;
	int margin_y = (this->height() - current_frame.rows) / 2;

	// Scale and subtract margins
	int mouse_x = mouse_point.x() - margin_x;
	int mouse_y = mouse_point.y() - margin_y;
	int scaled_x = mouse_x * current_frame.cols / (this->width() - margin_x * 2);
	int scaled_y = mouse_y * current_frame.rows / (this->height() - margin_y * 2);

	return QPoint(scaled_x, scaled_y);
}
