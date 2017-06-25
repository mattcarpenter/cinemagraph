#include "Cinemagraph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cinemagraph w;
    w.show();
    return a.exec();
}
