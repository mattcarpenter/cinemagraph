#pragma once

#include <QObject>
#include <qtimer.h>
#include "Composition.h"
#include <opencv2/opencv.hpp>
#include <qopenglfunctions.h>
#include <qopenglfunctions_3_0.h>
#include <qoffscreensurface.h>
#include <RenderWorker.h>
#include <qpoint.h>

class CinemagraphWorker : public QObject, protected QOpenGLFunctions_3_0
{
	Q_OBJECT

public:
	CinemagraphWorker(QOpenGLContext *context, QOffscreenSurface *surface);
	Composition* GetComposition();
	~CinemagraphWorker();

signals:
	void Frame(cv::Mat &frame);
	void TextureReady(GLuint tex, int pos, int video_length, int width, int height);
	void Thumbnail(cv::Mat);
	void LoopInPosition(int pos);
	void LoopOutPosition(int pos);

public slots:
	bool LoadVideo(std::string path);
	bool LoadStill(std::string path);
	void Initialize();
	void Play();
	void Pause();
	void Unpause();
	void RequestNextFrame();
	void Seek(int pos);
	void LoopIn();
	void LoopOut();
	void SetAsStill();
	void DeleteLayer(ILayer* layer);
	void SelectLayer(ILayer* layer);
	void MouseDown(QPoint p);
	void MouseUp(QPoint p);
	void MouseMove(QPoint p);
	void PaintBrushOn();
	void PaintBrushOff();
	void EraserOn();
	void EraserOff();
	void WheelTurn(int delta);

private:
	Composition *composition;
	MaskPainter *mask_painter;
	
	QOpenGLContext *q_opengl_context;
	QOffscreenSurface *q_surface;

	QThread *render_worker_thread;
	RenderWorker *render_worker;

	bool playing = false;
	bool last_playing_state = false;

	int current_frame = 0;

private slots:
	void OnTextureReady(GLuint tid, int pos, int video_length, int width, int height);
};
