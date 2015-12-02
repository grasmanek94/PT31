#ifndef ROBOTTASKQUEUE_H
#define ROBOTTASKQUEUE_H

#include <interprocess_queue/RawQueue.hxx>

template <size_t item_max_bytes = 2048, size_t per_queue_max_items = 128>
class RobotTaskQueue
{
	typedef RawQueue<item_max_bytes, per_queue_max_items> RTQ;
private:

	RTQ _ToDo;
	RTQ _Done;

public:

	RobotTaskQueue()
	{ }

	~RobotTaskQueue()
	{ }

	RTQ* ToDo()
	{
		return &_ToDo;
	}

	RTQ* Done()
	{
		return &_Done;
	}
};
#endif