#pragma once
#include <QThread>
#include <QString>
#include "Video.h"

class VideoWorker : public QObject
{
	Q_OBJECT
public:
	VideoWorker(Video* video);

public slots:
	void Stabilize();
	void GeneratePreview();
	void DetectFeatures();
	void AddMask(cv::Point, cv::Point);
signals:
	void frame(cv::Mat frame);

private:
	Video* video;
	cv::Mat current_frame;
	cv::Mat CurrentFrameWithMask();
};

