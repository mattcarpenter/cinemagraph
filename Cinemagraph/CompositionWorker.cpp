#include "CompositionWorker.h"
#include <qdebug.h>
#include <ctime>
#include <qthread.h>

using namespace cv;

CompositionWorker::CompositionWorker()
{
	composition = new Composition();
}

CompositionWorker::~CompositionWorker()
{
}

bool CompositionWorker::LoadVideo(std::string path)
{
	if (composition->LoadVideo(path))
	{
		end_frame = composition->FrameCount();
		return true;
	}

	return false;
}

bool CompositionWorker::LoadStill(std::string path)
{
	return composition->LoadStill(path);
}

void CompositionWorker::Test()
{
	//playback_timer_thread = std::thread(&CompositionWorker::update, this);
	//Mat frame;
	//composition->PopFrame(frame);
	//Render(frame);
	render_loop_thread = new QThread;
	render_worker = new RenderWorker(composition, q_opengl_context, q_surface);
	
	qDebug() << connect(render_loop_thread, SIGNAL(started()), render_worker, SLOT(Start()));
	qDebug() << connect(render_worker, &RenderWorker::TextureReady, this, &CompositionWorker::OnTextureReady);

	q_opengl_context->doneCurrent();
	q_opengl_context->moveToThread(render_loop_thread);
	q_surface->moveToThread(render_loop_thread);
	render_worker->moveToThread(render_loop_thread);

	render_loop_thread->start();
}

void CompositionWorker::OnTextureReady(GLuint tid)
{
	emit TextureReady(tid);
}

cv::Mat f;

void CompositionWorker::RenderLoop()
{
	Mat frame;
	while (1)
	{
		//Mat frame;
		int start_time = clock();
		composition->PopFrame(frame);
		Render(frame);
		//emit Frame(frame);
		int end_time = clock();
		int duration = (end_time - start_time) / double(CLOCKS_PER_SEC) * 1000;

		qDebug() << "duration: " << duration;
		this_thread::sleep_for(chrono::milliseconds(40 - duration));
	}
}

void CompositionWorker::Render(cv::Mat frame)
{
	if (frame.rows == 0)
		return;

	q_opengl_context->makeCurrent(q_surface);

	QOpenGLFunctions_3_0 *f = q_opengl_context->versionFunctions<QOpenGLFunctions_3_0>();
	int width = 500;
	int height = 500;
	matToTexture(frame, GL_NEAREST, GL_NEAREST, GL_CLAMP, texture_id);
	emit TextureReady(texture_id);
}

void CompositionWorker::matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, GLuint &tid)
{
	QOpenGLFunctions *f = q_opengl_context->functions();

	// Generate a number for our textureID's unique handle
	if (tid == 0)
	{
		f->glGenTextures(1, &tid);
	}

	// Bind to our texture handle
	f->glBindTexture(GL_TEXTURE_2D, tid);

	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
		magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		magFilter = GL_LINEAR;
	}

	// Set texture interpolation methods for minification and magnification
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Set texture clamping method
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_BGR;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}

	// Create the texture
	f->glTexImage2D(GL_TEXTURE_2D,     // Type of texture
		0,                 // Pyramid level (for mip-mapping) - 0 is the top level
		GL_RGB,            // Internal colour format to convert to
		mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
		mat.rows,          // Image height i.e. 480 for Kinect in standard mode
		0,                 // Border width in pixels (can either be 1 or 0)
		inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		GL_UNSIGNED_BYTE,  // Image data type
		mat.ptr());        // The actual image data itself
}