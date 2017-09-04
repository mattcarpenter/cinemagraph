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
	mask_painter = new MaskPainter(composition);
}

CinemagraphWorker::~CinemagraphWorker()
{
}

bool CinemagraphWorker::LoadVideo(std::string path)
{
	bool result = composition->LoadVideo(path, [&](cv::Mat frame) {
		emit Thumbnail(frame);
	});

	if (result)
	{
		//end_frame = composition->GetFrameCount();
		return true;
	}

	return false;
}

bool CinemagraphWorker::LoadStill(std::string path)
{
	return composition->LoadStill(path);
}

void CinemagraphWorker::Initialize()
{
	render_worker_thread = new QThread;
	render_worker = new RenderWorker(composition, mask_painter, q_opengl_context, q_surface);
	
	qDebug() << connect(render_worker_thread, SIGNAL(started()), render_worker, SLOT(Start()));
	qDebug() << connect(render_worker, &RenderWorker::TextureReady, this, &CinemagraphWorker::OnTextureReady);

	q_opengl_context->doneCurrent();
	q_opengl_context->moveToThread(render_worker_thread);
	q_surface->moveToThread(render_worker_thread);
	render_worker->moveToThread(render_worker_thread);

	render_worker_thread->start();
}

void CinemagraphWorker::OnTextureReady(GLuint tid, int pos, int video_length, int width, int height)
{
	current_frame = pos;
	emit TextureReady(tid, pos, video_length, width, height);
}

void CinemagraphWorker::Play()
{
	last_playing_state = playing = true;
	composition->SetPlaying(playing);
}

void CinemagraphWorker::Pause()
{
	last_playing_state = playing;
	playing = false;
	composition->SetPlaying(playing);
}

void CinemagraphWorker::RequestNextFrame()
{
	render_worker->sem->notify(2);
}

void CinemagraphWorker::Unpause()
{
	composition->SetPlaying(last_playing_state);
	playing = last_playing_state;
}

void CinemagraphWorker::Seek(int pos)
{
	composition->Seek(pos);
}

void CinemagraphWorker::LoopIn()
{
	composition->SetStartFrame(current_frame);
	emit LoopInPosition(current_frame);
}

void CinemagraphWorker::LoopOut()
{
	composition->SetEndFrame(current_frame);
	emit LoopOutPosition(current_frame);
}

void CinemagraphWorker::SetAsStill()
{
	composition->SetCurrentVideoFrameAsStill();
}

Composition* CinemagraphWorker::GetComposition()
{
	return composition;
}

void CinemagraphWorker::DeleteLayer(ILayer* layer)
{
	composition->DeleteLayer(layer);
}

void CinemagraphWorker::SelectLayer(ILayer* layer)
{
	if (layer->GetType() == LayerType::MASK)
	{
		mask_painter->AttachLayer((Mask*)layer);
	}
	else
	{
		mask_painter->DetachLayer();
	}
}

void CinemagraphWorker::MouseDown(QPoint p)
{
	mask_painter->MouseDown(p.x(), p.y());
}

void CinemagraphWorker::MouseMove(QPoint p)
{
	mask_painter->MouseMove(p.x(), p.y());
}

void CinemagraphWorker::MouseUp(QPoint p)
{
	mask_painter->MouseUp(p.x(), p.y());
}
