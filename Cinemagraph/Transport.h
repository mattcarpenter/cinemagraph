#pragma once

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <qlabel.h>
#include "TransportWorker.h"
#include <qpixmap.h>

class Transport : public QLabel
{
	Q_OBJECT

public:
	Transport(QWidget *parent);
	~Transport();

	void AppendThumbnail(cv::Mat thumb);
	void UpdatePosition(int pos, int video_length);
	void SetLoopInPosition(int position);
	void SetLoopOutPosition(int position);

private:
	int GetFrameNumberFromPoint(QPoint mouse_pos);

	QThread *transport_worker_thread;
	TransportWorker *transport_worker;
	QPixmap pixmap;
	int position = 0;
	int frame_count = 0;

private slots:
	void resizeEvent(QResizeEvent *);
	void OnFrame(QPixmap pm);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);

signals:
	void Thumbnail(cv::Mat thumb);
	void Resize(int w, int h);
	void PositionUpdated(int pos, int video_length);
	void TransportMousePress(int pos);
	void TransportMouseRelease();
	void TransportMouseMove(int pos);
	void LoopInPosition(int position);
	void LoopOutPosition(int position);
};
