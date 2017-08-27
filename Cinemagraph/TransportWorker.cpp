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
	thumbnails.push_back(thumb);
	Render();
}

void TransportWorker::PositionUpdated(int pos, int vid_length)
{
	position = pos;
	video_length = vid_length;
	Render();
}

void TransportWorker::Render()
{
	if (thumbnails.size() == 0)
		return;

	const int thumb_width = thumbnails[0].cols;
	const int thumb_height = thumbnails[0].rows;

	Mat out(height, width, CV_8UC3, Scalar(34, 34, 34));

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

	if (video_length > 0)
	{
		// Draw current position marker
		int x = min(width - 1, width * position / video_length);
		rectangle(out, Rect(x, 0, 2, height), Scalar(255, 45, 197), 1);
	
		// Draw loop in marker
		rectangle(out, Rect(GetLoopInOffset(), 0, 1, height), Scalar(255, 255, 255), 1);

		// Draw loop out marker
		rectangle(out, Rect(GetLoopOutOffset(), 0, 1, height), Scalar(255, 255, 255), 1);
	}

	

	// Draw borders
	rectangle(out, Rect(0, 0, width, 1), Scalar(0, 0, 0), 1);
	rectangle(out, Rect(0, 0, 1, height), Scalar(0, 0, 0), 1);
	rectangle(out, Rect(0, height - 1, width, height), Scalar(90, 90, 90), 1);
	rectangle(out, Rect(width - 1, 0, 1, height), Scalar(90, 90, 90), 1);

	QPixmap pm = ASM::cvMatToQPixmap(out);
	emit Frame(pm);
}

void TransportWorker::Resize(int w, int h)
{
	width = w;
	height = h;
	Render();
}

void TransportWorker::LoopInPosition(int position)
{
	loop_in_position = position;
	Render();
}

void TransportWorker::LoopOutPosition(int position)
{
	loop_out_position = position;
	Render();
}

int TransportWorker::GetLoopInOffset()
{
	return min(width - 1, width * loop_in_position / video_length);
}

int TransportWorker::GetLoopOutOffset()
{
	return min(width - 1, width * loop_out_position / video_length);
}