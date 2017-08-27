#include "Transport.h"
#include <qlabel.h>
#include <qthread.h>
#include <qdebug.h>
#include <qpixmap.h>
#include <qevent.h>

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
	qDebug() << connect(this, SIGNAL(PositionUpdated(int, int)), transport_worker, SLOT(PositionUpdated(int, int)));
	qDebug() << connect(this, SIGNAL(LoopInPosition(int)), transport_worker, SLOT(LoopInPosition(int)));
	qDebug() << connect(this, SIGNAL(LoopOutPosition(int)), transport_worker, SLOT(LoopOutPosition(int)));

	transport_worker->moveToThread(transport_worker_thread);
	transport_worker_thread->start();
}

void Transport::UpdatePosition(int pos, int video_length)
{
	position = pos;
	frame_count = video_length;
	PositionUpdated(pos, video_length);
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

void Transport::mouseReleaseEvent(QMouseEvent * event)
{
	QPoint marquee_end = event->pos();
	emit TransportMouseRelease();
}

void Transport::mousePressEvent(QMouseEvent * event)
{
	emit TransportMousePress(GetFrameNumberFromPoint(event->pos()));
}

void Transport::mouseMoveEvent(QMouseEvent * event)
{
	emit TransportMouseMove(GetFrameNumberFromPoint(event->pos()));
}

void Transport::OnFrame(QPixmap pm)
{
	pixmap = pm;
	this->setPixmap(pm);
}

int Transport::GetFrameNumberFromPoint(QPoint mouse_pos)
{
	return mouse_pos.x() * frame_count / this->width();
}

void Transport::SetLoopInPosition(int position)
{
	emit LoopInPosition(position);
}

void Transport::SetLoopOutPosition(int position)
{
	emit LoopOutPosition(position);
}
