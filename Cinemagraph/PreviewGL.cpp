#include "PreviewGL.h"
#include <qdebug.h>
#include <qevent.h>

PreviewGL::PreviewGL(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

PreviewGL::~PreviewGL()
{
}

void PreviewGL::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	emit Initialized();
}

void PreviewGL::resizeGL(int w, int h)
{
	update();
}

void PreviewGL::paintGL()
{
	if (texture_id == 0)
		return;
	
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	GLuint readFboId = 0;
	glGenFramebuffers(1, &readFboId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture_id, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
	cv::Rect crop_area = GetDrawSize();
	glBlitFramebuffer(0, 0, texture_width, texture_height,
		crop_area.x, crop_area.y, crop_area.width, crop_area.height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void PreviewGL::Test(cv::Mat frame)
{
	frame.copyTo(current_frame);
	update();
}

void PreviewGL::TextureReady(GLuint tid, int width, int height)
{
	texture_id = tid;
	texture_width = width;
	texture_height = height;
	update();
}

GLuint PreviewGL::matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
	// Generate a number for our textureID's unique handle
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Bind to our texture handle
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
		magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		magFilter = GL_LINEAR;
	}

	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

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
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
		0,                 // Pyramid level (for mip-mapping) - 0 is the top level
		GL_RGB,            // Internal colour format to convert to
		mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
		mat.rows,          // Image height i.e. 480 for Kinect in standard mode
		0,                 // Border width in pixels (can either be 1 or 0)
		inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		GL_UNSIGNED_BYTE,  // Image data type
		mat.ptr());        // The actual image data itself

	return textureID;
}

cv::Rect PreviewGL::GetDrawSize()
{
	cv::Rect ret;
	int this_width = this->width();
	int this_height = this->height();

	// If we scale the video down to fit the entire width of the view, will the proportional
	// height be less than the view? if so, make the video the full width of the view and scale
	// the height.
	if (this_width * texture_height / texture_width < this_height)
	{
		// borders on top and bottom
		// use frame width, calculated height
		ret.height = this_width * texture_height / texture_width;
		ret.width = this->width();
		ret.y = (this_height - ret.height) / 2;
		ret.height += ret.y;
	}
	else
	{
		// borders on left and right
		// use frame height, calculated width
		ret.height = this->height();
		ret.width = texture_width * this_height / texture_height;
		ret.x = (this_width - ret.width) / 2;
		ret.width += ret.x;
	}

	return ret;
}

void PreviewGL::mousePressEvent(QMouseEvent * event)
{
	qDebug() << MouseToVideoPoint(event->pos());
}

QPoint PreviewGL::MouseToVideoPoint(QPoint mouse_point)
{
	if (texture_width == 0 || texture_height == 0)
		return QPoint(0, 0);

	int this_width = this->width();
	int this_height = this->height();

	int margin_x;
	int margin_y;

	if (this_width > texture_width && this_height > texture_height)
	{
		// image is smaller than the label, so it's placed in the center and scaled 1:1
		margin_x = (this_width - texture_width) / 2;
		margin_y = (this_height - texture_height) / 2;
		int mouse_x = mouse_point.x() - margin_x;
		int mouse_y = mouse_point.y() - margin_y;

		return QPoint(mouse_x, mouse_y);
	}

	// TODO - "zoom" calculation

	// Image has been scaled down
	int scaled_width, scaled_height;
	if (this_width * texture_height / texture_width > this_height)
	{
		// Margins are on the left and right
		scaled_width = texture_width * this_height / texture_height;
		scaled_height = this_height;
		margin_y = 0;
		margin_x = (this_width - scaled_width) / 2;
	}
	else
	{
		// Margins are on the top and bottom
		scaled_width = this_width;
		scaled_height = texture_height * this_width / texture_width;
		margin_y = (this_height - scaled_height) / 2;
		margin_x = 0;
	}

	// Scale and subtract margins
	int mouse_x = mouse_point.x() - margin_x;
	int mouse_y = mouse_point.y() - margin_y;
	int scaled_x = mouse_x * texture_width / scaled_width;
	int scaled_y = mouse_y * texture_height / scaled_height;

	return QPoint(scaled_x, scaled_y);
}
