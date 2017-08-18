#include "TransportWorker.h"
#include <qdebug.h>
#include "asmOpenCV.h"

using namespace std;
using namespace cv;

TransportWorker::TransportWorker(int w, int h)
{
	width = w;
	height = h;
}

TransportWorker::~TransportWorker()
{
}

void TransportWorker::AppendThumbnail(Mat thumb)
{
	qDebug() << "transport worker got thumb with w: " << thumb.cols;
	thumbnails.push_back(thumb);
	Render();
}

void TransportWorker::Render()
{
	if (thumbnails.size() == 0)
		return;

	const int thumb_width = thumbnails[0].cols;
	const int thumb_height = thumbnails[0].rows;

	Mat out(height, width, CV_8UC3, Scalar(0, 0, 0));
	
	if (thumbnails.size() * thumb_width < width)
	{
		// Not enough thumbnails to fill transport
		for (int i = 0; i < thumbnails.size(); i++)
		{
			thumbnails[i].copyTo(out(Rect(i * thumb_width, 0, thumbnails[i].cols, thumbnails[i].rows)));
		}
	}
	else
	{
		// More than enough thumbnails to fill transport
		const int fits = width / thumb_width;
		const int stride = thumbnails.size() / fits;
		for (int i = 0; i < fits; i++)
		{
			thumbnails[i * stride].copyTo(out(Rect(i * thumb_width, 0, thumbnails[i].cols, thumbnails[i].rows)));
		}

		// Fill in remaining space
		const int margin = width - (fits * thumb_width);
		thumbnails[(fits - 1) * stride](Rect(0, 0, margin, thumb_height)).copyTo(out(Rect(fits * thumb_width, 0, margin, thumb_height)));
	}

	QPixmap pm = ASM::cvMatToQPixmap(out);
	emit Frame(pm);
}

void TransportWorker::Resize(int w, int h)
{
	width = w;
	height = h;
	Render();
}
