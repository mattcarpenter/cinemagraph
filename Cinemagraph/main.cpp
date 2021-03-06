#include "Cinemagraph.h"
#include <QtWidgets/QApplication>
#include <qstylefactory.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication::setStyle("motif");
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QPalette p;
	p = qApp->palette();
	p.setColor(QPalette::Window, QColor(53, 53, 53));
	p.setColor(QPalette::Button, QColor(53, 53, 53));
	p.setColor(QPalette::Highlight, QColor(142, 45, 197));
	p.setColor(QPalette::ButtonText, QColor(210, 210, 210));
	p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(175, 175, 175));
	p.setColor(QPalette::WindowText, QColor(210, 210, 210));
	qApp->setPalette(p);

	Cinemagraph w;
	w.show();
	return a.exec();
}
