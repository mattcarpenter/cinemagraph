#include "CinemagraphWorker.h"
#include <qdebug.h>
#include <ctime>
#include <qthread.h>

using namespace cv;

CinemagraphWorker::CinemagraphWorker(QOpenGLContext *context, QOffscreenSurface *surface)
{
	q_opengl_context = context;
	q_surface = surface;
	composition = new Composition();
}

CinemagraphWorker::~CinemagraphWorker()
{
}

bool CinemagraphWorker::LoadVideo(std::string path)
{
	if (composition->LoadVideo(path))
	{
		//end_frame = composition->GetFrameCount();
		return true;
	}
}

bool CinemagraphWorker::LoadStill(std::string path)
{
	return composition->LoadStill(path);
}

void CinemagraphWorker::Initialize()
{
	render_worker_thread = new QThread;
	render_worker = new RenderWorker(composition, q_opengl_context, q_surface);
	
	qDebug() << connect(render_worker_thread, SIGNAL(started()), render_worker, SLOT(Start()));
	qDebug() << connect(render_worker, &RenderWorker::TextureReady, this, &CinemagraphWorker::OnTextureReady);

	q_opengl_context->doneCurrent();
	q_opengl_context->moveToThread(render_worker_thread);
	q_surface->moveToThread(render_worker_thread);
	render_worker->moveToThread(render_worker_thread);

	render_worker_thread->start();
}

void CinemagraphWorker::OnTextureReady(GLuint tid)
{
	emit TextureReady(tid);
}
