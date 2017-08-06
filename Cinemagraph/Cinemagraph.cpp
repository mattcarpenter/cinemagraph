#include "Cinemagraph.h"
#include <qthread.h>
#include <qdebug.h>
#include <qfiledialog.h>

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(QOpenGLContext*);
Q_DECLARE_METATYPE(GLuint);

using namespace cv;

Cinemagraph::Cinemagraph(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<cv::Mat>("cv::Mat&");
	qRegisterMetaType<QOpenGLContext*>("QOpenGLContext*");
	qRegisterMetaType<GLuint>("GLuint");
}

void Cinemagraph::OpenGLInitialized()
{
	QOpenGLContext *context = new QOpenGLContext();
	QOpenGLContext *shareContext = ui.preview_gl->context();
	context->setFormat(shareContext->format());
	context->setShareContext(shareContext);
	context->create();

	QOffscreenSurface *surface = new QOffscreenSurface();
	surface->setFormat(context->format());
	surface->create();

	composition_thread = new QThread;
	composition_worker = new CompositionWorker();

	composition_worker->q_opengl_context = context;
	composition_worker->q_surface = surface;

	//qDebug() << connect(videoWorker, &VideoWorker::frame, this, &StabilizeWindow::on_videoFrame_updated);
	//qDebug() << connect(videoThread, SIGNAL(started()), videoWorker, SLOT(GeneratePreview()));
	qDebug() << connect(this, SIGNAL(LoadVideo(std::string)), composition_worker, SLOT(LoadVideo(std::string)));
	qDebug() << connect(this, SIGNAL(LoadStill(std::string)), composition_worker, SLOT(LoadStill(std::string)));
	qDebug() << connect(this, SIGNAL(Test()), composition_worker, SLOT(Test()));
	qDebug() << connect(composition_worker, &CompositionWorker::Frame, this, &Cinemagraph::OnCompositionFrame);
	qDebug() << connect(composition_worker, &CompositionWorker::TextureReady, this, &Cinemagraph::OnTextureReady);
	composition_worker->moveToThread(composition_thread);
	composition_worker->q_opengl_context->moveToThread(composition_thread);
	composition_worker->q_surface->moveToThread(composition_thread);
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

void Cinemagraph::on_new_mask_layer_clicked()
{	
	emit Test();
}

void Cinemagraph::OnTextureReady(GLuint tid)
{
	ui.preview_gl->TextureReady(tid);
}

void Cinemagraph::OnCompositionFrame(cv::Mat &frame)
{
	//ui.preview->DrawFrame(frame);
	ui.preview_gl->Test(frame);
}
