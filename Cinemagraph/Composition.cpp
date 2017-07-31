#include "Composition.h"
#include <qdebug.h>

using namespace std;
using namespace cv;

Composition::Composition()
{
	video_layer = new Layer();
	still_layer = new Layer();
}

Composition::~Composition()
{
}

bool Composition::LoadVideo(string path)
{
	return video_layer->LoadVideo(path);
}

bool Composition::LoadStill(string path)
{
	return still_layer->LoadImage(path);
}

bool Composition::Render(int frame_number, Mat &target)
{
	Mat still_frame, still_overlay;
	Mat video_frame, video_overlay;

	still_layer->RenderFrame(frame_number, still_frame, still_overlay);
	video_layer->RenderFrame(frame_number, video_frame, video_overlay);

	// TODO - merge still and video frames; push result into frame queue
	if (video_frame.cols > 0)
	{
		video_frame.copyTo(target);
		return true;
	}

	return false;
}
