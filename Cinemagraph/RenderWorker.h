#pragma once

#include <QObject>
#include "Composition.h"
#include "MaskPainter.h"
#include <opencv2/opencv.hpp>
#include <qopenglfunctions.h>
#include <qopenglfunctions_4_5_core.h>
#include <qoffscreensurface.h>

class RenderWorker : public QObject, protected QOpenGLFunctions_4_5_Core
{
	Q_OBJECT

public:
	RenderWorker(Composition *comp, MaskPainter *mpainter, QOpenGLContext *ctx, QSurface *sfc);
	~RenderWorker();
	void SetSeeking(bool seeking);

	Semaphore *sem;
public slots:
	void Start();

private:
	void PrepareTexture(cv::Mat frame, GLuint texture_id);
	void matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, GLuint &tid);

	Composition *composition;
	MaskPainter *mask_painter;

	QOpenGLContext *q_opengl_context;
	QSurface *q_surface;

	const int texture_count = 1;
	int current_texture_index = 0;
	GLuint *texture_ids;
	bool initial_texture_created = false;

	bool is_seeking = false;
	
signals:
	void TextureReady(GLuint tid, int pos, int video_length, int width, int height);
};
