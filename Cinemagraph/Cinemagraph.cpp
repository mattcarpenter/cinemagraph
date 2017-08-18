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

/**
 * SLOT: OpenGLInitialized()
 * 
 * Connected to Initialized signal from PreviewGL. Fires when the QOpenGLWidget has initialized
 * and a QOpenGLContext has been created. This method will create a shared QOpenGLContext and
 * initialize the CinemagraphWorker in a separate thread.
 */
void Cinemagraph::OpenGLInitialized()
{
	// Create a new QOpenGLContext for the worker thread and share with the PreviewGL widget's context
	QOpenGLContext *context = new QOpenGLContext();
	QOpenGLContext *shareContext = ui.preview_gl->context();
	context->setFormat(shareContext->format());
	context->setShareContext(shareContext);
	context->create();

	QOffscreenSurface *surface = new QOffscreenSurface();
	surface->setFormat(context->format());
	surface->create();

	// Create the Cinemgraph worker object and thread
	cinemagraph_worker_thread = new QThread;
	cinemagraph_worker = new CinemagraphWorker(context, surface);

	// Connect slots and signals between the main thread and the Cinemagraph worker
	qDebug() << connect(cinemagraph_worker_thread, SIGNAL(started()), cinemagraph_worker, SLOT(Initialize()));
	qDebug() << connect(this, SIGNAL(LoadVideo(std::string)), cinemagraph_worker, SLOT(LoadVideo(std::string)));
	qDebug() << connect(this, SIGNAL(LoadStill(std::string)), cinemagraph_worker, SLOT(LoadStill(std::string)));
	qDebug() << connect(this, SIGNAL(Play()), cinemagraph_worker, SLOT(Play()));
	qDebug() << connect(this, SIGNAL(Pause()), cinemagraph_worker, SLOT(Pause()));
	qDebug() << connect(ui.preview_gl, SIGNAL(RequestNextFrame()), cinemagraph_worker, SLOT(RequestNextFrame()));
	qDebug() << connect(cinemagraph_worker, &CinemagraphWorker::TextureReady, this, &Cinemagraph::OnTextureReady);
	qDebug() << connect(cinemagraph_worker, &CinemagraphWorker::Thumbnail, this, &Cinemagraph::OnThumbnail);

	// Move the worker, context, and surface to the worker thread
	cinemagraph_worker->moveToThread(cinemagraph_worker_thread);
	context->moveToThread(cinemagraph_worker_thread);
	surface->moveToThread(cinemagraph_worker_thread);

	// Start the worker thread. The Cinemagraph worker will create its own render thread that emits an OpenGL
	// texture ID back to the main thread, triggering a redraw of the PreviewGL widget.
	cinemagraph_worker_thread->start();
}

void Cinemagraph::OnThumbnail(cv::Mat thumb)
{
	ui.transport_bar->AppendThumbnail(thumb);
}

/**
 * SLOT: on_load_video_clicked()
 *
 * fired when the `Load Video` button is clicked
 */
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

/**
 * SLOT: on_load_still_clicked()
 *
 * Fired when the `Load Still` button is clicked.
 */
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

/**
 * SLOT: on_new_mask_layer_clicked()
 *
 * Fired when the `New Mask Layer` button is clicked
 */
void Cinemagraph::on_new_mask_layer_clicked()
{	
	//emit Test();
}

/**
 * SLOT: OnTextureReady(GLuint tid)
 *
 * Connected to the `TextureReady` signal emitted by the CinemagraphWorker
 * Signals that a new composition frame has been uploaded by the worker thread
 * into OpenGL and should be rendered by the PreviewGL widget.
 */
void Cinemagraph::OnTextureReady(GLuint tid, int pos, int width, int height)
{
	// Notify the PreviewGL widget that a new composition frame (texture) is ready
	// to be rendered.
	ui.preview_gl->TextureReady(tid, width, height);
}

void Cinemagraph::on_play_button_clicked()
{
	emit Play();
}

void Cinemagraph::on_pause_button_clicked()
{
	emit Pause();
}
