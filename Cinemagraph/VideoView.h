#pragma once

#include <QLabel>
#include <opencv2/opencv.hpp>

class VideoView : public QLabel
{
	Q_OBJECT

public:
	VideoView(QWidget *parent);
	~VideoView();
	void SetVideoFrame(cv::Mat frame);
	void SetMask(cv::Mat mask);
	void PaintVideoPreview();

private:
	cv::Mat current_frame;
	cv::Mat preview_buffer;
	cv::Mat mask;
	QPoint marquee_start;
	QPoint marquee_end;
	QPoint MouseToVideoPoint(QPoint mouse_point);
protected slots:
	virtual void enterEvent(QEvent * event);
	virtual void leaveEvent(QEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);

signals:
	void AddMask(QPoint, QPoint);
};
