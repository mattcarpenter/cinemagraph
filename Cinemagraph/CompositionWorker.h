#pragma once

#include <QObject>
#include <qtimer.h>
#include "Composition.h"
#include <opencv2/opencv.hpp>

class CompositionWorker : public QObject
{
	Q_OBJECT

public:
	CompositionWorker();
	~CompositionWorker();

public slots:
	bool LoadVideo(std::string path);
	bool LoadStill(std::string path);

private:
	Composition *composition;
	QTimer *render_timer;
	
	int current_frame = 0;
	int queue_max_size = 3;
	int start_frame = 0;
	int end_frame = 0;

	std::queue<cv::Mat> frame_queue;

private slots:
	void render();
};
