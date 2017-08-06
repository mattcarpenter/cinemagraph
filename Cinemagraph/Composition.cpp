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

void Composition::Render(Mat &target)
{
	// TODO - Render still layer and blend with video
	video_layer->Render(target);
}

int Composition::GetFrameCount()
{
	return max(video_layer->GetFrameCount(), still_layer->GetFrameCount());
}

void Composition::SetStartFrame(int sf)
{
	start_frame = sf;
}

void Composition::SetEndFrame(int ef)
{
	end_frame = ef;
}

void Composition::SetPlaying(bool playing)
{
	is_playing = playing;
}
