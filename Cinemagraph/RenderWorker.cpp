#include "RenderWorker.h"
#include "qdebug.h"
#include "qpixmap.h"
#include "asmOpenCV.h"

using namespace cv;

RenderWorker::RenderWorker(Composition *comp, QOpenGLContext *ctx, QSurface *sfc)
{
	composition = comp;
	q_surface = sfc;
	q_opengl_context = ctx;
	sem = new Semaphore(1);
}

RenderWorker::~RenderWorker()
{
}

void RenderWorker::SetSeeking(bool seeking)
{
	is_seeking = seeking;
}

void RenderWorker::Start()
{
	Mat frame;

	q_opengl_context->makeCurrent(q_surface);

	QOpenGLFunctions_4_5_Core *f = q_opengl_context->versionFunctions<QOpenGLFunctions_4_5_Core>();
	GLuint tid;
	f->glGenTextures(texture_count, &tid);

	while (1)
	{
		// wait for UI to request another frame
		sem->wait(1);

		int start_time = clock();
		
		// Render the next composition frame
		int video_pos = composition->Render(frame);

		// Get frame count
		int video_length = composition->GetFrameCount();

		if (frame.cols > 0)
			cv::flip(frame, frame, 0);

		// Load into an OpenGL texture
		PrepareTexture(frame, tid);
		
		// Signal to the UI thread that the texture can now be applied to a quad
		// and rendered by the PreviewGL widget.
		emit TextureReady(tid, video_pos, video_length, frame.cols, frame.rows);

		int end_time = clock();
		int duration = (end_time - start_time) / double(CLOCKS_PER_SEC) * 1000;
		
		// Try to keep this block executing every 40ms
		// TODO - Use framerate of composition
		this_thread::sleep_for(chrono::milliseconds(40 - duration));
		//qDebug() << "duration: " << duration;
		current_texture_index++;
		if (current_texture_index >= texture_count)
		{
			current_texture_index = 0;
		}
	}
}

/**
 * Loads a cv::Mat into an OpenGL texture within a context shared with the PreviewGL
 * widget on the main thread.
 */
void RenderWorker::PrepareTexture(cv::Mat frame, GLuint texture_id)
{
	if (frame.rows == 0)
		return;
	q_opengl_context->makeCurrent(q_surface);
	QOpenGLFunctions_4_5_Core *f = q_opengl_context->versionFunctions<QOpenGLFunctions_4_5_Core>();
	int width = 500;
	int height = 500;
	matToTexture(frame, GL_NEAREST, GL_NEAREST, GL_CLAMP, texture_id);
}

/**
 * Loads a cv::Mat into an OpenGL texture
 *
 * @param {cv::Mat&} mat Source image
 * @param {GLenum} minFilter
 * @param {GLenum} magFilter
 * @param {GLenum} wrapFilter
 * @param {GLuint&} tid
 */
void RenderWorker::matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, GLuint &tid)
{
	QOpenGLFunctions_4_5_Core *f = q_opengl_context->versionFunctions<QOpenGLFunctions_4_5_Core>();

	// Bind to our texture handle
	f->glBindTexture(GL_TEXTURE_2D, tid);

	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_BGR;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}

	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
		magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		magFilter = GL_LINEAR;
	}

	// Set texture interpolation methods for minification and magnification
	/*f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Set texture clamping method
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	f->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	*/

	if (!initial_texture_created)
	{
		f->glTexImage2D(GL_TEXTURE_2D,     // Type of texture
			0,                 // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,            // Internal colour format to convert to
			mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
			mat.rows,          // Image height i.e. 480 for Kinect in standard mode
			0,                 // Border width in pixels (can either be 1 or 0)
			inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,  // Image data type
			mat.ptr());        // The actual image data itself
		initial_texture_created = true;
	} 
	else
	{
		f->glTexSubImage2D(GL_TEXTURE_2D,
			0,
			0,
			0,
			mat.cols,
			mat.rows,
			inputColourFormat,
			GL_UNSIGNED_BYTE,
			mat.ptr());
	}
	f->glFinish();
	f->glBindTexture(GL_TEXTURE_2D, 0);
}
