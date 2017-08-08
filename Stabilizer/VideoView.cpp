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
	int frame_width = pmFrame.width();
	int frame_height = pmFrame.height();
	int this_width = this->width();
	int this_height = this->height();
	this->setAlignment(Qt::AlignCenter);
	if (frame_width > this_width || frame_height > this_height)
		QLabel::setPixmap(pmFrame.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else
		setPixmap(pmFrame);
}

void VideoView::resizeEvent(QResizeEvent* event)
{
	QPixmap pmFrame = ASM::cvMatToQPixmap(preview_buffer);
	int frame_width = pmFrame.width();
	int frame_height = pmFrame.height();
	int this_width = this->width();
	int this_height = this->height();
	this->setAlignment(Qt::AlignCenter);
	if (frame_width > this_width || frame_height > this_height)
		QLabel::setPixmap(pmFrame.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else
		setPixmap(pmFrame);
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
	rectangle(preview_buffer, Rect(Point(marquee_start_video.x(), marquee_start_video.y()), Point(marquee_current_video.x(), marquee_current_video.y())), Scalar(50, 200, 255), 1);
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

	int frame_width = current_frame.cols;
	int frame_height = current_frame.rows;
	int this_width = this->width();
	int this_height = this->height();

	int margin_x;
	int margin_y;

	if (this_width > frame_width && this_height > frame_height)
	{
		// image is smaller than the label, so it's placed in the center and scaled 1:1
		margin_x = (this_width - frame_width) / 2;
		margin_y = (this_height - frame_height) / 2;
		int mouse_x = mouse_point.x() - margin_x;
		int mouse_y = mouse_point.y() - margin_y;

		return QPoint(mouse_x, mouse_y);
	}

	// TODO - "zoom" calculation

	// Image has been scaled down
	int scaled_width, scaled_height;
	if (this_width * frame_height / frame_width > this_height)
	{
		// Margins are on the left and right
		scaled_width = frame_width * this_height / frame_height;
		scaled_height = this_height;
		margin_y = 0;
		margin_x = (this_width - scaled_width) / 2;
	}
	else
	{
		// Margins are on the top and bottom
		scaled_width = this_width;
		scaled_height = frame_height * this_width / frame_width;
		margin_y = (this_height - scaled_height) / 2;
		margin_x = 0;
	}

	// Scale and subtract margins
	int mouse_x = mouse_point.x() - margin_x;
	int mouse_y = mouse_point.y() - margin_y;
	int scaled_x = mouse_x * current_frame.cols / scaled_width;
	int scaled_y = mouse_y * current_frame.rows / scaled_height;

	return QPoint(scaled_x, scaled_y);
}
