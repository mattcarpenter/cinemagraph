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
bool Layer::LoadVideo(string path, function<void(cv::Mat)> thumb_callback)
{
	Mat frame;
	bool result;

	layer_type = LayerType::VIDEO;
	
	video_capture = new VideoCapture(path);
	if (!video_capture->isOpened())
		qDebug() << "could not open video.";
	
	// Initial processing of video
	// Counts the number of frames, generates thumbnails, etc.
	if (result = video_capture->read(frame))
	{
		// PASS 1: Count frames
		video_capture_frame_count = 1;
		while (video_capture->read(frame))
			video_capture_frame_count++;

		end_frame = video_capture_frame_count;

		// reset position
		video_capture->set(CV_CAP_PROP_POS_FRAMES, 0);

		// PASS 2: Generate thumbnails
		int current_frame = 0;
		while (video_capture->read(frame))
		{
			if (current_frame % (video_capture_frame_count / thumb_count) == 0)
			{
				Mat resized;
				int w = frame.cols * thumb_height / frame.rows;
				int h = thumb_height;
				resize(frame, resized, Size(w, h), 0, 0, INTER_AREA);
				thumb_callback(resized);
			}

			current_frame++;
		}

		// reset position
		video_capture->set(CV_CAP_PROP_POS_FRAMES, 0);

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

int Layer::RenderNextFrame(cv::Mat &frame)
{
	// NOTE - Executed within RenderWorker thread

	int captured_frame_pos = 0;

	if (layer_type == LayerType::STILL)
	{
		still.copyTo(frame);
		// TODO - Process (mask, adjustments, etc)

		return 0;
	}
	else
	{
		// Block this render if the CaptureLoop is currently seeking
		while (seek_to_frame > -1)
		{
			capture_sem->notify(2);
		}

		// Wait for the capture thread to push a frame if needed
		if (capture_queue.size() == 0)
		{
			while (capture_queue.size() == 0)
			{
				this_thread::sleep_for(chrono::milliseconds(1));
			}
		}
		
		CaptureFrame *cf = capture_queue.front();
		
		if (this->GetVisible())
			cf->GetFrame(frame);
		else
			this->GetBlank(frame);

		captured_frame_pos = cf->GetFrameNumber();
		// TODO - Process (mask, adjustments, etc)
		
		if (is_playing)
		{
			capture_queue.pop();
			delete cf;

			// notify capture thread that we've popped a frame
			capture_sem->notify(2);
		}

		// TODO - figure out how to get the index of the last frame read from the front
		// of the queue, not the index of the last frame captured from the video.
		return captured_frame_pos;
	}
}

void Layer::CaptureLoop()
{
	Mat frame;
	int last_captured_frame_pos = 0;

	while (1)
	{
		if (capture_queue.size() >= capture_queue_max_length && seek_to_frame == -1)
		{
			// wait for the consumer to pop a frame off the queue
			capture_sem->wait(1);
			continue;
		}

		if (seek_to_frame > -1)
		{
			// Seeking
			video_capture->set(CV_CAP_PROP_POS_FRAMES, seek_to_frame);
			
			// Empty the capture queue
			while (capture_queue.size() > 0)
			{
				delete capture_queue.front();
				capture_queue.pop();
			}

			// Adjust our state
			last_captured_frame_pos = seek_to_frame;
			current_frame_number = seek_to_frame;
			seek_to_frame = -1;
		}

		video_capture->read(frame);
		capture_queue.push(new CaptureFrame(frame, last_captured_frame_pos));
		capture_sem->notify(1);

		if (++current_frame_number >= min(end_frame, video_capture_frame_count))
		{
			current_frame_number = start_frame;
			last_captured_frame_pos = start_frame;
			bool success = video_capture->set(CV_CAP_PROP_POS_FRAMES, start_frame);
		}
		else
		{
			last_captured_frame_pos++;
		}
	}
}

/**
 * Seeks to a specified position in the video. NOOP if layer is a still
 */
void Layer::Seek(int pos)
{
	if (layer_type == LayerType::STILL)
		return;
	seek_to_frame = pos;
}

LayerType Layer::GetType()
{
	return layer_type;
}

bool Layer::GetVisible()
{
	return visible;
}

void Layer::SetVisible(bool v)
{
	visible = v;
}

void Layer::GetBlank(cv::Mat frame)
{
	if (blank.cols != frame.cols || blank.rows != frame.rows)
	{
		// Create new blank
		blank = Mat(frame.rows, frame.cols, CV_8UC3, Scalar(0, 0, 0));
	}
	
	blank.copyTo(frame);
	
}
