#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cinemagraph.h"
#include "CinemagraphWorker.h"

class Cinemagraph : public QMainWindow
{
	Q_OBJECT

public:
	Cinemagraph(QWidget *parent = Q_NULLPTR);

public slots:
	void on_load_video_clicked();
	void on_load_still_clicked();
	void on_new_mask_layer_clicked();
	void OpenGLInitialized();
	void OnTextureReady(GLuint tid);

private:
	Ui::CinemagraphClass ui;

	QThread *cinemagraph_worker_thread;
	CinemagraphWorker *cinemagraph_worker;

signals:
	void LoadVideo(std::string path);
	void LoadStill(std::string path);
	void Test();
};
