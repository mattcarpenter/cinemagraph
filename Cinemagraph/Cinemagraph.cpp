#include "Cinemagraph.h"
#include "StabilizeWindow.h"
#include <iostream>

using namespace std;

Cinemagraph::Cinemagraph(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Cinemagraph::on_stabilizeButton_clicked()
{
	stabilizeWindow.show();
}