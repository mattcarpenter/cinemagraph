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
	void PositionUpdated(int pos, int vid_length);
	void LoopInPosition(int position);
	void LoopOutPosition(int position);

private:
	int width;
	int height;
	int position = 0;
	int video_length = 0;

	void Render();

	std::vector<cv::Mat> thumbnails;
	
	int GetLoopInOffset();
	int GetLoopOutOffset();

	int loop_in_position = 0;
	int loop_out_position = 0;
signals:
	void Frame(QPixmap pm);
};
