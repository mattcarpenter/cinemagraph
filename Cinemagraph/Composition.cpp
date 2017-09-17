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

void Composition::AddMask(Mask* mask)
{
	masks.push_back(mask);
}

int Composition::GetWidth()
{
	return std::max(video_layer->GetWidth(), still_layer->GetWidth());
}

int Composition::GetHeight()
{
	return std::max(video_layer->GetHeight(), still_layer->GetHeight());
}

int Composition::Render(Mat &target)
{
	int video_pos = 0;
	
	// NOTE - This is executed by RenderWorker thread
	//
	// TODO - These callback functions smell. I wanted to avoid declaring
	//        cv::Mats within this function scope for performance reasons
	//        but I suspect something like the following would effectively
	//        be the same in terms of performance:
	//			cv::Mat frame = video_layer->RenderNextFrame();
	//        Keep in mind, the RenderNextFrame() method must also give us
	//        the position (frame number). Yeah, this smells too.

	video_layer->RenderNextFrame([&](int pos, cv::Mat frame) -> void {
		video_pos = pos;

		still_layer->RenderNextFrame([&](int pos, cv::Mat still) -> void {

			// Do we have a video frame and a still frame?
			if (still.rows > 0 && frame.rows > 0) {

				// Begin hardware-accelerated alpha blending of video frame and still frame
				
				// Initialize GpuMat vectors to store individual channel data for the frame
				// and still, then upload our 8UC3 images to the GPU.
				std::vector<GpuMat> gpu_dest_channels;
				std::vector<GpuMat> gpu_still_channels;
				std::vector<GpuMat> gpu_frame_channels;
				gpu_frame.upload(frame);
				gpu_still.upload(still);
				
				// Merge all mask layers into `mask` and `mask_highlight` before uploading
				// the mask Mats to the GPU.
				UpdateMask();
				
				gpu_mask.upload(mask);
				gpu_mask_highlight.upload(mask_highlight);

				// Convert video frame to 8UC4 and give it an all-white mask
				cuda::split(gpu_frame, gpu_frame_channels);
				if (gpu_frame_mask.cols != frame.cols || gpu_frame_mask.rows != frame.cols)
					gpu_frame_mask = GpuMat(frame.rows, frame.cols, CV_8UC1, Scalar(255));
				gpu_frame_channels.push_back(gpu_frame_mask);
				cuda::merge(gpu_frame_channels, gpu_frame_with_alpha);

				// Replace the still image alpha channel with the mask
				cuda::split(gpu_still, gpu_still_channels);
				gpu_still_channels.pop_back();
				gpu_still_channels.push_back(gpu_mask);
				// Add highlights to still
				cuda::addWeighted(gpu_still_channels.at(2),
					1.0, gpu_mask_highlight, 0.25, 1.0, gpu_still_channels.at(2));
				// Merge still channels into single 8CU4
				cuda::merge(gpu_still_channels, gpu_still);

				// Compose the video frame and still frame
				cuda::alphaComp(gpu_still, gpu_frame_with_alpha, gpu_dest_with_alpha, ALPHA_OVER);
				
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

/**
 * Updates the member mask variable with a composite of all mask layers
 */
void Composition::UpdateMask()
{
	bool initialized = false;

	// TODO - Don't re-generate a composite mask if none of the mask layers
	//        have been flagged as dirty.

	// TODO - All this logic probably belongs in the MaskPainter.
	//        Also, MaskPainter probably should be wrapped up into the
	//        Layer class somehow.
	
	// Ensure our mask composition always has proper dimensions
	if (mask.cols != GetWidth() || mask.rows != GetHeight()
		|| masks.size() > 0)
	{
		mask = Mat::zeros(GetHeight(), GetWidth(), CV_8UC1);
		mask.copyTo(mask_highlight);
	}

	// Loop through each mask and merge into `mask` and `mask_highlight`
	for (int i = 0; i < masks.size(); i++)
	{
		Mask *m = masks.at(i);
		if (!m->GetVisible())
			continue;

		mask = cv::max(mask, m->GetMat());
		if (m->IsEditing())
		{
			if (m->GetPaintMode() == PaintMode::PAINT_BRUSH)
			{
				mask = cv::max(m->GetPreview(), mask);
				mask = cv::max(m->GetCommitted(), mask);
			}
			else if (m->GetPaintMode() == PaintMode::ERASER)
			{
				mask = cv::min(mask, m->GetPreview());
				mask = cv::min(mask, m->GetCommitted());
			}
		}

		if (m->GetHighlighted() && m->GetVisible())
		{
			mask_highlight = cv::max(mask_highlight, m->GetMat());
			if (m->IsEditing())
			{
				mask_highlight = cv::max(mask_highlight, m->GetPreview());
				mask_highlight = cv::max(mask_highlight, m->GetCommitted());
			}
		}
	}
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
	// TODO - Don't use the GPU for this operation. It only happens once during a user's
	//        workflow and it'll just make it more difficult to make hardware acceleration
	//        an optional feature later on.

	Mat still;
	video_layer->GetCurrentFrame(still);
	
	// Convert to 8CU4
	// First, create an all-white mask
	GpuMat gpu_still(still);
	GpuMat gpu_mask(still.rows, still.cols, CV_8UC1, Scalar(255));
	GpuMat gpu_dest;

	// Merge RGB channels with Alpha
	std::vector<GpuMat> gpu_still_channels;
	cuda::split(gpu_still, gpu_still_channels);
	gpu_still_channels.push_back(gpu_mask);
	cuda::merge(gpu_still_channels, gpu_dest);

	// Download 8CU4 from the GPU
	gpu_dest.download(still);
	still_layer->LoadImage(still);
}

void Composition::DeleteLayer(ILayer* layer)
{
	// TODO - Enhance to support the deletion of any layer type. Presently, this only
	//        matches against mask layers.
	
	// TODO - Make this thread safe. There's currently a race condition during which
	//        we attempt to manipulate the masks vector while the render worker thread
	//        is enumerating the masks and generating a composite for the still image's
	//        alpha channel.

	for (auto m : masks)
	{
		if (layer == m)
		{
			masks.erase(std::remove(masks.begin(), masks.end(), m), masks.end());
			break;
		}
	}

	// If no masks remain, blank out the composite mask member variable
	if (!masks.size())
	{
		mask = Mat::zeros(GetHeight(), GetWidth(), CV_8UC1);
	}
}
