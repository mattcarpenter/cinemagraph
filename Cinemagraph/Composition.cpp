#include "Composition.h"
#include <opencv2/cudaimgproc.hpp>
#include <qdebug.h>
#include <Mask.h>

using namespace std;
using namespace cv;
using namespace cv::cuda;

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
	int video_pos = 0;
	// NOTE - Executed within RenderWorker thread
	// TODO - Render still layer and blend with video
	
	video_layer->RenderNextFrame([&](int pos, cv::Mat frame) -> void {
		video_pos = pos;

		still_layer->RenderNextFrame([&](int pos, cv::Mat still) -> void {

			// Do we have a video frame and a still frame?
			if (still.rows > 0 && frame.rows > 0) {

				// TODO - mask should live in the video layer
				if (mask.cols == 0)
				{
					mask = Mat(still.rows, still.cols, CV_8UC1, Scalar(255));
					rectangle(mask, Rect(20, 20, 200, 200), Scalar(0), -1);
					for (int z = 220; z < 220 + 255; z++) {
						line(mask, Point(z, 20), Point(z, 220), Scalar(z - 220));
					}
				}

				// Upload images to GPU
				gpu_frame.upload(frame);
				gpu_still.upload(still);
				gpu_mask.upload(mask);

				// Convert video frame to 8UC4 and use the mask as the alpha channel
				std::vector<GpuMat> gpu_frame_channels;
				std::vector<GpuMat> gpu_dest_channels;
				cuda::split(gpu_frame, gpu_frame_channels);
				gpu_frame_channels.push_back(gpu_mask);
				cuda::merge(gpu_frame_channels, gpu_frame_with_alpha);

				// Compose the video frame and still frame
				cuda::alphaComp(gpu_frame_with_alpha, gpu_still, gpu_dest_with_alpha, ALPHA_OVER);
				
				// Drop the alpha channel from the blended image
				cuda::split(gpu_dest_with_alpha, gpu_dest_channels);
				gpu_dest_channels.pop_back();
				cuda::merge(gpu_dest_channels, gpu_dest);

				// Download to the render target
				gpu_dest.download(target);

			}
			else if (frame.rows > 0 && !still.rows)
			{
				// Have video; no still
				frame.copyTo(target);
			}
			else if (!frame.rows && still.rows)
			{
				// Have still; no video
				gpu_still.upload(still);
				std::vector<GpuMat> gpu_still_channels;
				cuda::split(gpu_still, gpu_still_channels);
				gpu_still_channels.pop_back(); // removes alpha
				cuda::merge(gpu_still_channels, gpu_dest);
				gpu_dest.download(target);
			}
		});
	});

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

void Composition::SetCurrentVideoFrameAsStill()
{
	Mat still;
	video_layer->GetCurrentFrame(still);
	
	// Convert to 8CU4
	GpuMat gpu_still(still);
	GpuMat gpu_mask(still.rows, still.cols, CV_8UC1, Scalar(255));
	GpuMat gpu_dest;

	// convert frame to 8UC4
	std::vector<GpuMat> gpu_still_channels;
	cuda::split(gpu_still, gpu_still_channels);
	gpu_still_channels.push_back(gpu_mask);
	cuda::merge(gpu_still_channels, gpu_dest);
	gpu_dest.download(still);
	still_layer->LoadImage(still);
}