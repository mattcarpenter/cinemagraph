#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions.h>
#include <qopenglfunctions_3_0.h>
#include <opencv2/opencv.hpp>

class PreviewGL : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
	Q_OBJECT

public:
	PreviewGL(QWidget *parent);
	~PreviewGL();
	void Test(cv::Mat frame);
	void TextureReady(GLuint tid);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
	cv::Mat current_frame;
	int last_time = 0;
	GLuint texture_id = 0;

signals:
	void Initialized();
};
