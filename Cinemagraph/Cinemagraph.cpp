#include "Cinemagraph.h"
#include <qthread.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qpoint.h>
#include <qicon.h>

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(QOpenGLContext*);
Q_DECLARE_METATYPE(GLuint);
Q_DECLARE_METATYPE(ILayer*);
Q_DECLARE_METATYPE(QPoint);

using namespace cv;

Cinemagraph::Cinemagraph(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<cv::Mat>("cv::Mat&");
	qRegisterMetaType<QOpenGLContext*>("QOpenGLContext*");
	qRegisterMetaType<GLuint>("GLuint");
	qRegisterMetaType<ILayer*>("ILayer*");
	qRegisterMetaType<QPoint>("QPoint");
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

	ui.project_tree->AddVideoLayer(cinemagraph_worker->GetComposition()->GetVideoLayer());
	ui.project_tree->AddStillLayer(cinemagraph_worker->GetComposition()->GetStillLayer());
	
	// Connect slots and signals between the main thread and the Cinemagraph worker
	connect(cinemagraph_worker_thread, SIGNAL(started()), cinemagraph_worker, SLOT(Initialize()));
	connect(this, SIGNAL(LoadVideo(std::string)), cinemagraph_worker, SLOT(LoadVideo(std::string)));
	connect(this, SIGNAL(LoadStill(std::string)), cinemagraph_worker, SLOT(LoadStill(std::string)));
	connect(this, SIGNAL(Play()), cinemagraph_worker, SLOT(Play()));
	connect(this, SIGNAL(Pause()), cinemagraph_worker, SLOT(Pause()));
	connect(this, SIGNAL(SetAsStill()), cinemagraph_worker, SLOT(SetAsStill()));
	connect(ui.preview_gl, SIGNAL(RequestNextFrame()), cinemagraph_worker, SLOT(RequestNextFrame()));
	connect(cinemagraph_worker, &CinemagraphWorker::TextureReady, this, &Cinemagraph::OnTextureReady);
	connect(cinemagraph_worker, &CinemagraphWorker::Thumbnail, this, &Cinemagraph::OnThumbnail);
	
	// PreviewGL connections
	connect(ui.preview_gl, SIGNAL(MouseDown(QPoint)), cinemagraph_worker, SLOT(MouseDown(QPoint)));
	connect(ui.preview_gl, SIGNAL(MouseMove(QPoint)), cinemagraph_worker, SLOT(MouseMove(QPoint)));
	connect(ui.preview_gl, SIGNAL(MouseUp(QPoint)), cinemagraph_worker, SLOT(MouseUp(QPoint)));
	connect(ui.preview_gl, SIGNAL(WheelTurn(int)), cinemagraph_worker, SLOT(WheelTurn(int)));

	// Slider connections
	qDebug() << connect(ui.brush_hardness, SIGNAL(valueChanged(int)), cinemagraph_worker, SLOT(BrushHardness(int)));

	// Transport connections
	connect(ui.transport_bar, SIGNAL(TransportMouseRelease()), cinemagraph_worker, SLOT(Unpause()));
	connect(ui.transport_bar, SIGNAL(TransportMousePress(int)), this, SLOT(OnTransportSeekStart(int)));
	connect(ui.transport_bar, SIGNAL(TransportMouseMove(int)), cinemagraph_worker, SLOT(Seek(int)));
	connect(this, SIGNAL(Seek(int)), cinemagraph_worker, SLOT(Seek(int)));

	// Loop in/out connections
	connect(this, SIGNAL(LoopOut()), cinemagraph_worker, SLOT(LoopOut()));
	connect(this, SIGNAL(LoopIn()), cinemagraph_worker, SLOT(LoopIn()));
	connect(cinemagraph_worker, SIGNAL(LoopOutPosition(int)), this, SLOT(LoopOutPosition(int)));
	connect(cinemagraph_worker, SIGNAL(LoopInPosition(int)), this, SLOT(LoopInPosition(int)));

	// Project Tree connections
	connect(ui.project_tree, SIGNAL(DeleteLayer(ILayer*)), cinemagraph_worker, SLOT(DeleteLayer(ILayer*)));
	connect(ui.project_tree, SIGNAL(SelectLayer(ILayer*)), cinemagraph_worker, SLOT(SelectLayer(ILayer*)));

	// Toolbar connections
	connect(ui.toolbar, SIGNAL(PaintBrushOn()), cinemagraph_worker, SLOT(PaintBrushOn()));
	connect(ui.toolbar, SIGNAL(PaintBrushOff()), cinemagraph_worker, SLOT(PaintBrushOff()));
	connect(ui.toolbar, SIGNAL(EraserOn()), cinemagraph_worker, SLOT(EraserOn()));
	connect(ui.toolbar, SIGNAL(EraserOff()), cinemagraph_worker, SLOT(EraserOff()));

	// Move the worker, context, and surface to the worker thread
	cinemagraph_worker->moveToThread(cinemagraph_worker_thread);
	context->moveToThread(cinemagraph_worker_thread);
	surface->moveToThread(cinemagraph_worker_thread);

	// Start the worker thread. The Cinemagraph worker will create its own render thread that emits an OpenGL
	// texture ID back to the main thread, triggering a redraw of the PreviewGL widget.
	cinemagraph_worker_thread->start();
}

void Cinemagraph::OnTransportSeekStart(int pos)
{
	// Mouse-down on the Transport should pause the video _and_ seek to the requested position
	emit Pause();
	emit Seek(pos);
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
void Cinemagraph::on_new_mask_layer_button_clicked()
{	
	int width = cinemagraph_worker->GetComposition()->GetWidth();
	int height = cinemagraph_worker->GetComposition()->GetHeight();

	if (!width || !height)
		return;

	Mask *m = new Mask(width, height, "Mask");
	cinemagraph_worker->GetComposition()->AddMask(m);
	ui.project_tree->AddMask(m);
}

/**
 * SLOT: OnTextureReady(GLuint tid)
 *
 * Connected to the `TextureReady` signal emitted by the CinemagraphWorker
 * Signals that a new composition frame has been uploaded by the worker thread
 * into OpenGL and should be rendered by the PreviewGL widget.
 */
void Cinemagraph::OnTextureReady(GLuint tid, int pos, int video_length, int width, int height)
{
	// Notify the PreviewGL widget that a new composition frame (texture) is ready
	// to be rendered.
	ui.preview_gl->TextureReady(tid, width, height);
	ui.transport_bar->UpdatePosition(pos, video_length);
	ui.project_tree->update();
}

void Cinemagraph::on_set_as_still_button_clicked()
{
	emit SetAsStill();
}

void Cinemagraph::on_play_button_clicked()
{
	emit Play();
}

void Cinemagraph::on_pause_button_clicked()
{
	emit Pause();
}

void Cinemagraph::on_loop_in_button_clicked()
{
	emit LoopIn();
}

void Cinemagraph::on_loop_out_button_clicked()
{
	emit LoopOut();
}

void Cinemagraph::LoopInPosition(int position)
{
	ui.transport_bar->SetLoopInPosition(position);
}

void Cinemagraph::LoopOutPosition(int position)
{
	ui.transport_bar->SetLoopOutPosition(position);
}
