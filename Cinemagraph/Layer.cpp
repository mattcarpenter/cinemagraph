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
	
	if (result = video_capture->read(frame))
	{
		// get length
		video_capture_frame_count = 1;

		while (video_capture->read(frame))
			video_capture_frame_count++;

		// reset position
		video_capture->set(CV_CAP_PROP_POS_FRAMES, 0);
		layer_type = LayerType::VIDEO;

		// Begin the capture thread
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
 * Returns the number of frames contained within this layer
 *
 * @return {int} layer count
 */
int Layer::FrameCount()
{
	if (layer_type == LayerType::STILL)
		return 1;
	else if (layer_type == LayerType::VIDEO)
		return video_capture_frame_count;
	else
		return -1;
}

/**
 * Renders layer at given frame position
 *
 */
/*void Layer::RenderFrame(int pos, Mat &frame, Mat &overlay)
{
	if (video_capture == NULL || !video_capture->isOpened())
		return;
		
	// FFMPEG prefers that we don't try to explicitly increment the frames,
	// especially for heavily-compressed source mataterial such as H.264.
	if (pos == 0)
		video_capture->set(CV_CAP_PROP_POS_FRAMES, pos);
	video_capture->read(frame);
}*/

void Layer::PopFrame(cv::Mat &frame)
{
	capture_queue.front().copyTo(frame);
	capture_queue.pop();
	capture_sem->notify(2);
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