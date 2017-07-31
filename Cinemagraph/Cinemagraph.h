#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cinemagraph.h"
#include "CompositionWorker.h"

class Cinemagraph : public QMainWindow
{
	Q_OBJECT

public:
	Cinemagraph(QWidget *parent = Q_NULLPTR);

public slots:
	void on_load_video_clicked();
	void on_load_still_clicked();

private:
	Ui::CinemagraphClass ui;

	QThread *composition_thread;
	CompositionWorker *composition_worker;

signals:
	void LoadVideo(std::string path);
	void LoadStill(std::string path);
};
