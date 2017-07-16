#include "StabilizeWindow.h"
#include <QFileDialog>
#include "Video.h"
#include <opencv2/opencv.hpp>
#include "VideoWorker.h"
#include <qthread.h>
#include "qdebug.h"

using namespace cv;

Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(cv::Point);

StabilizeWindow::StabilizeWindow(QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<cv::Point>("cv::Point");
	ui.setupUi(this);
	
}

StabilizeWindow::~StabilizeWindow()
{
}

void StabilizeWindow::on_videoFrame_updated(cv::Mat frame)
{
	ui.preview->SetVideoFrame(frame);
	ui.preview->PaintVideoPreview();

	if (!loaded) {
		loaded = true;
		ui.detect_features_button->setEnabled(true);
	}
}

void StabilizeWindow::on_detect_features_button_clicked()
{
	emit Stabilize();
}

void StabilizeWindow::on_addMask(QPoint a, QPoint b)
{
	emit AddMask(Point(a.x(), a.y()), Point(b.x(), b.y()));
}

void StabilizeWindow::on_loadButton_clicked()
{
	QFileDialog dialog(this);
	QStringList fileNames;

	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setViewMode(QFileDialog::Detail);
	
	if (!dialog.exec())
		return;

	fileNames = dialog.selectedFiles();

	if (fileNames.size() != 1)
		return;

	QString filePath = fileNames.at(0);

	this->video = new Video("C:\\Users\\mattc\\Desktop\\temp\\scaled.mp4");

	// Create the worker and start in a separate thread
	videoThread = new QThread;
	videoWorker = new VideoWorker(this->video);
	videoWorker->moveToThread(videoThread);
	qDebug() << connect(videoWorker, &VideoWorker::frame, this, &StabilizeWindow::on_videoFrame_updated);
	qDebug() << connect(videoThread, SIGNAL(started()), videoWorker, SLOT(GeneratePreview()));
	qDebug() << connect(this, SIGNAL(DetectFeatures()), videoWorker, SLOT(DetectFeatures()));
	qDebug() << connect(this, SIGNAL(Stabilize()), videoWorker, SLOT(Stabilize()));
	qDebug() << connect(this, SIGNAL(AddMask(cv::Point, cv::Point)), videoWorker, SLOT(AddMask(cv::Point, cv::Point)));
	videoThread->start();
}
