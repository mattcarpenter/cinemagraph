#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cinemagraph.h"
#include "StabilizeWindow.h"

class Cinemagraph : public QMainWindow
{
    Q_OBJECT

public:
    Cinemagraph(QWidget *parent = Q_NULLPTR);

private:
    Ui::CinemagraphClass ui;
	StabilizeWindow stabilizeWindow;

private slots:
	void on_stabilizeButton_clicked();
};
