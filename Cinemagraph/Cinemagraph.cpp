#include "Cinemagraph.h"
#include <qthread.h>
#include <qdebug.h>
#include <qfiledialog.h>

Q_DECLARE_METATYPE(std::string);

Cinemagraph::Cinemagraph(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<std::string>("std::string");

	composition_thread = new QThread;
	composition_worker = new CompositionWorker();
	composition_worker->moveToThread(composition_thread);
	//qDebug() << connect(videoWorker, &VideoWorker::frame, this, &StabilizeWindow::on_videoFrame_updated);
	//qDebug() << connect(videoThread, SIGNAL(started()), videoWorker, SLOT(GeneratePreview()));
	qDebug() << connect(this, SIGNAL(LoadVideo(std::string)), composition_worker, SLOT(LoadVideo(std::string)));
	qDebug() << connect(this, SIGNAL(LoadStill(std::string)), composition_worker, SLOT(LoadStill(std::string)));
	composition_thread->start();
}

void Cinemagraph::on_load_video_clicked()
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
	string str_file_path = filePath.toStdString();

	emit LoadVideo((char*)str_file_path.c_str());
}

void Cinemagraph::on_load_still_clicked()
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
	string str_file_path = filePath.toStdString();

	emit LoadStill((char*)str_file_path.c_str());
}