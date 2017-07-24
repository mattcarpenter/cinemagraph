#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cinemagraph.h"

class Cinemagraph : public QMainWindow
{
	Q_OBJECT

public:
	Cinemagraph(QWidget *parent = Q_NULLPTR);

private:
	Ui::CinemagraphClass ui;
};
