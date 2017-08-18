#include "Transport.h"
#include <qlabel.h>
#include <qthread.h>
#include <qdebug.h>
#include "qpixmap.h"

using namespace std;
using namespace cv;

Q_DECLARE_METATYPE(QPixmap);

Transport::Transport(QWidget *parent)
	: QLabel(parent)
{
	qRegisterMetaType<QPixmap>("QPixmap");

	transport_worker_thread = new QThread;
	transport_worker = new TransportWorker(this->width(), this->height());

	qDebug() << connect(this, SIGNAL(Thumbnail(cv::Mat)), transport_worker, SLOT(AppendThumbnail(cv::Mat)));
	qDebug() << connect(this, SIGNAL(Resize(int, int)), transport_worker, SLOT(Resize(int, int)));
	qDebug() << connect(transport_worker, SIGNAL(Frame(QPixmap)), this, SLOT(OnFrame(QPixmap)));

	transport_worker->moveToThread(transport_worker_thread);
	transport_worker_thread->start();
}

Transport::~Transport()
{
}

void Transport::AppendThumbnail(Mat thumb)
{
	emit Thumbnail(thumb);
}

void Transport::resizeEvent(QResizeEvent *)
{
	emit Resize(this->width(), this->height());
	QLabel::setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

void Transport::OnFrame(QPixmap pm)
{
	pixmap = pm;
	this->setPixmap(pm);
}