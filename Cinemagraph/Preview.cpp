#include "Preview.h"
#include "asmOpenCV.h"
#include "qdebug.h"

Preview::Preview(QWidget *parent)
	: QLabel(parent)
{
}

Preview::~Preview()
{
}

void Preview::DrawFrame(cv::Mat frame)
{
	int start = clock();
	QPixmap pmFrame = ASM::cvMatToQPixmap(frame);
	int frame_width = pmFrame.width();
	int frame_height = pmFrame.height();
	int label_width = this->width();
	int label_height = this->height();
	this->setAlignment(Qt::AlignCenter);
	if (frame_width > label_width || frame_height > label_height)
		QLabel::setPixmap(pmFrame.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else
		setPixmap(pmFrame);

	int end = clock();

	qDebug() << "old preview process time: " << (end - start) / double(CLOCKS_PER_SEC) * 1000;

}
