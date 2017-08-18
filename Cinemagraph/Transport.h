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

private:
	QThread *transport_worker_thread;
	TransportWorker *transport_worker;
	QPixmap pixmap;

private slots:
	void resizeEvent(QResizeEvent *);
	void OnFrame(QPixmap pm);
signals:
	void Thumbnail(cv::Mat thumb);
	void Resize(int w, int h);
};
