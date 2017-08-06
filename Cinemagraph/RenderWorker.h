#pragma once

#include <QObject>
#include "Composition.h"
#include <opencv2/opencv.hpp>
#include <qopenglfunctions.h>
#include <qopenglfunctions_3_0.h>
#include <qoffscreensurface.h>

class RenderWorker : public QObject
{
	Q_OBJECT

public:
	RenderWorker(Composition *comp, QOpenGLContext *ctx, QSurface *sfc);
	~RenderWorker();

public slots:
	void Start();

private:
	Composition *composition;

	void Render(cv::Mat frame);
	void matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, GLuint &tid);

	QOpenGLContext *q_opengl_context;
	QSurface *q_surface;
	GLuint texture_id;

signals:
	void TextureReady(GLuint tid);
};
