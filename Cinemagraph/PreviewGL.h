#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions.h>
#include <qopenglfunctions_4_5_core.h>

#include <opencv2/opencv.hpp>

class PreviewGL : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
	Q_OBJECT

public:
	PreviewGL(QWidget *parent);
	~PreviewGL();
	void Test(cv::Mat frame);
	void TextureReady(GLuint tid, int width, int height);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	QPoint MouseToVideoPoint(QPoint mouse_point);
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
	cv::Rect GetDrawSize();
	cv::Mat current_frame;
	GLuint texture_id = 0;
	int texture_width = 0;
	int texture_height = 0;
signals:
	void Initialized();
	void RequestNextFrame();
	void MouseDown(QPoint p);
	void MouseMove(QPoint p);
	void MouseUp(QPoint p);
	void WheelTurn(int delta);
};
