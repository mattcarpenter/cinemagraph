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
	void on_play_button_clicked();
	void on_pause_button_clicked();
	void on_loop_in_button_clicked();
	void on_loop_out_button_clicked();
	void on_set_as_still_button_clicked();
	void OpenGLInitialized();
	void OnTextureReady(GLuint tid, int pos, int video_length, int width, int height);
	void OnThumbnail(cv::Mat thumb);
	void LoopInPosition(int pos);
	void LoopOutPosition(int pos);
	void OnTransportSeekStart(int pos);

private:
	Ui::CinemagraphClass ui;

	QThread *cinemagraph_worker_thread;
	CinemagraphWorker *cinemagraph_worker;

signals:
	void LoadVideo(std::string path);
	void LoadStill(std::string path);
	void Test();
	void Play();
	void Pause();
	void LoopIn();
	void LoopOut();
	void Seek(int pos);
	void SetAsStill();
};
