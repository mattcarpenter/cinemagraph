#include "Layer.h"
#include <qdebug.h>

using namespace std;
using namespace cv;

Layer::Layer()
{
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
void Layer::RenderFrame(int pos, Mat &frame, Mat &overlay)
{
	if (video_capture == NULL || !video_capture->isOpened())
		return;

	video_capture->set(CV_CAP_PROP_POS_FRAMES, pos);
	video_capture->read(frame);
	qDebug() << "read";
}
