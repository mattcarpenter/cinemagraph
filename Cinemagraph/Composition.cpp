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

bool Composition::LoadVideo(string path, function<void(cv::Mat)> thumb_callback)
{
	return video_layer->LoadVideo(path, thumb_callback);
}

bool Composition::LoadStill(string path)
{
	return still_layer->LoadImage(path);
}

int Composition::Render(Mat &target)
{
	// NOTE - Executed within RenderWorker thread

	// TODO - Render still layer and blend with video
	int video_pos = video_layer->RenderNextFrame(target);

	return video_pos;
}

int Composition::GetFrameCount()
{
	return max(video_layer->GetFrameCount(), still_layer->GetFrameCount());
}

void Composition::SetStartFrame(int sf)
{
	start_frame = sf;
	video_layer->SetStartFrame(sf);
}

void Composition::SetEndFrame(int ef)
{
	end_frame = ef;
	video_layer->SetEndFrame(ef);
}

int Composition::GetStartFrame()
{
	return start_frame;
}

int Composition::GetEndFrame()
{
	return end_frame;
}

void Composition::Seek(int pos)
{
	video_layer->Seek(pos);
}

void Composition::SetPlaying(bool playing)
{
	is_playing = playing;
	video_layer->SetPlaying(is_playing);
}

Layer* Composition::GetVideoLayer()
{
	return video_layer;
}

Layer* Composition::GetStillLayer()
{
	return still_layer;
}
