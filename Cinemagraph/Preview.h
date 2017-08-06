#pragma once

#include <QWidget>
#include <qlabel.h>
#include <opencv2/opencv.hpp>

class Preview : public QLabel
{
	Q_OBJECT

public:
	Preview(QWidget *parent);
	void Preview::DrawFrame(cv::Mat frame);
	~Preview();
};
