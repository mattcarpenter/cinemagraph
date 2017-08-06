#include "Layer.h"
#include <qdebug.h>

using namespace std;
using namespace cv;

Layer::Layer()
{
	capture_sem = new Semaphore(1);
}

Layer::~Layer()
{
}

/**
 * Loads a video into the layer
 *
 * @param {string} path
 * @return {bool}
 */
bool Layer::LoadVideo(string path)
{
	Mat frame;
	bool result;

	video_capture = new VideoCapture(path);
	if (!video_capture->isOpened())
		qDebug() << "could not open video.";
	
	// Initial processing of video
	// Counts the number of frames, generates thumbnails, etc.
	// TODO - Generate thumbnails for transport
	if (result = video_capture->read(frame))
	{
		// get length
		video_capture_frame_count = 1;

		while (video_capture->read(frame))
			video_capture_frame_count++;

		end_frame = video_capture_frame_count;

		// reset position
		video_capture->set(CV_CAP_PROP_POS_FRAMES, 0);
		layer_type = LayerType::VIDEO;

		// Begin the continuous capture thread
		capture_thread = std::thread(&Layer::CaptureLoop, this);
		
	}
	return result;
}

/**
 * Loads an image into the layer
 *
 * @param {string} path
 * @return bool
 */
bool Layer::LoadImage(string path)
{
	still = imread(path, CV_LOAD_IMAGE_COLOR);
	if (still.rows > 0 && still.cols > 0)
	{
		layer_type = LayerType::STILL;
		return true;
	}
	else
		return false;

}

/**
 * Sets the playback loop starting frame
 *
 * @param {int} sf
 */
void Layer::SetStartFrame(int sf)
{
	start_frame = max(0, min(sf, video_capture_frame_count));
}

/**
 * Sets the playback loop ending frame
 *
 * @param {int} ef
 */
void Layer::SetEndFrame(int ef)
{
	end_frame = max(0, min(ef, video_capture_frame_count));
}

/**
 * Sets whether or not each call to Render() advances to the next frame
 *
 * @param {bool} playing
 */
void Layer::SetPlaying(bool playing)
{
	is_playing = playing;
}

/**
 * Returns the number of frames in the video, or 1 if the layer contains a still
 *
 * @return {int} frame count
 */
int Layer::GetFrameCount()
{
	if (layer_type == LayerType::STILL)
		return 1;
	else if (layer_type == LayerType::VIDEO)
		return video_capture_frame_count;
	else
		return -1;
}

void Layer::Render(cv::Mat &frame)
{
	if (layer_type == LayerType::STILL)
	{
		still.copyTo(frame);
		// TODO - Process (mask, adjustments, etc)
	}
	else
	{
		// Wait for the capture thread to push a frame if needed
		if (capture_queue.size() == 0)
		{
			while (capture_queue.size() == 0)
			{
				this_thread::sleep_for(chrono::milliseconds(1));
			}
		}

		capture_queue.front().copyTo(frame);
		// TODO - Process (mask, adjustments, etc)
		
		if (is_playing)
		{
			capture_queue.pop();
			capture_sem->notify(2);
		}
	}
}

void Layer::CaptureLoop()
{
	Mat frame;
	while (1)
	{
		if (capture_queue.size() >= capture_queue_max_length)
		{
			// wait for the consumer to pop a frame off the queue
			capture_sem->wait(1);
			continue;
		}

		video_capture->read(frame);
		capture_sem->notify(1);
		capture_queue.push(frame.clone());

		if (++current_frame_number >= video_capture_frame_count)
		{
			current_frame_number = 0;
			video_capture->set(CV_CAP_PROP_POS_FRAMES, 0);
		}
	}
}
