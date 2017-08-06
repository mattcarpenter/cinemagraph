#pragma once

#include <QObject>
#include <qtimer.h>
#include "Composition.h"
#include <opencv2/opencv.hpp>
#include <qopenglfunctions.h>
#include <qopenglfunctions_3_0.h>
#include <qoffscreensurface.h>

class CompositionWorker : public QObject, protected QOpenGLFunctions_3_0
{
	Q_OBJECT

public:
	CompositionWorker();
	~CompositionWorker();

	QOpenGLContext *q_opengl_context;
	QOffscreenSurface *q_surface;

signals:
	void Frame(cv::Mat &frame);
	void TextureReady(GLuint tex);

public slots:
	bool LoadVideo(std::string path);
	bool LoadStill(std::string path);
	void Test();

private:

	void Render(cv::Mat frame);
	void matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, GLuint &tid);
	GLuint texture_id = 0;

	Composition *composition;

	int current_frame = 0;
	int start_frame = 0;
	int end_frame = 0;

	std::thread playback_timer_thread;

private slots:
	void update();
};
