#pragma once

#include <QObject>
#include <qtimer.h>
#include "Composition.h"
#include <opencv2/opencv.hpp>
#include <qopenglfunctions.h>
#include <qopenglfunctions_3_0.h>
#include <qoffscreensurface.h>
#include <RenderWorker.h>

class CinemagraphWorker : public QObject, protected QOpenGLFunctions_3_0
{
	Q_OBJECT

public:
	CinemagraphWorker(QOpenGLContext *context, QOffscreenSurface *surface);
	~CinemagraphWorker();

signals:
	void Frame(cv::Mat &frame);
	void TextureReady(GLuint tex, int pos);

public slots:
	bool LoadVideo(std::string path);
	bool LoadStill(std::string path);
	void Initialize();

private:
	Composition *composition;
	
	QOpenGLContext *q_opengl_context;
	QOffscreenSurface *q_surface;

	QThread *render_worker_thread;
	RenderWorker *render_worker;

private slots:
	void OnTextureReady(GLuint tid, int pos);
};
