#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include <qpixmap.h>

class TransportWorker : public QObject
{
	Q_OBJECT

public:
	TransportWorker(int w, int h);
	~TransportWorker();

public slots:
	void AppendThumbnail(cv::Mat thumb);
	void Resize(int w, int h);

private:
	int width;
	int height;

	void Render();

	std::vector<cv::Mat> thumbnails;

signals:
	void Frame(QPixmap pm);
};
