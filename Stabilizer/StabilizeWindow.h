#pragma once

#include <QMainWindow>
#include "ui_StabilizeWindow.h"
#include <opencv2/opencv.hpp>
#include "Video.h"
#include "VideoWorker.h"

class StabilizeWindow : public QMainWindow
{
	Q_OBJECT

public:
	StabilizeWindow(QWidget *parent = Q_NULLPTR);
	~StabilizeWindow();

private:
	Ui::StabilizeWindow ui;
	Video* video;
	VideoWorker* stabilizeWorker;
	bool loaded;

	QThread* videoThread;
	VideoWorker* videoWorker;

private slots:
	void on_loadButton_clicked();
	void on_detect_features_button_clicked();
	void on_reset_mask_button_clicked();
	void on_stabilize_button_clicked();

signals:
	void DetectFeatures();
	void Stabilize();
	void AddMask(cv::Point, cv::Point);
	void ResetMask();

public slots:
	void on_videoFrame_updated(cv::Mat frame);
	void on_addMask(QPoint, QPoint);
};
