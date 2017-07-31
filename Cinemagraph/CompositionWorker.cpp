#include "CompositionWorker.h"

using namespace cv;

CompositionWorker::CompositionWorker()
{
	composition = new Composition();

	render_timer = new QTimer(this);
	connect(render_timer, SIGNAL(timeout()), this, SLOT(render()));
	render_timer->start(1);
}

CompositionWorker::~CompositionWorker()
{
}

bool CompositionWorker::LoadVideo(std::string path)
{
	return composition->LoadVideo(path);
}

bool CompositionWorker::LoadStill(std::string path)
{
	return composition->LoadStill(path);
}

void CompositionWorker::render()
{
	// Only render a new frame if there's room in the queue for it
	if (frame_queue.size() >= queue_max_size)
		return;

	// Lock the mutex to insure no updates updates to the composition or layers
	// occur while we're in the process of rendering and merging them.
	//mtx.lock();
	Mat target;
	if (composition->Render(0, target))
	{
		frame_queue.push(target.clone());

		// Update current_frame counter
		if (++current_frame > end_frame)
			current_frame = start_frame;
	}
	// Unlock mutex to resume pending updates to the composition and layers
	//mtx.unlock();	
}
