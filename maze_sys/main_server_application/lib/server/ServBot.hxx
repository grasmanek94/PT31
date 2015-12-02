#ifndef SERVBOT_H
#define SERVBOT_H

#include "RobotTaskQueue.hxx"

template <size_t item_max_bytes = 2048, size_t per_queue_max_items = 128>
class ServBot
{
	typedef RobotTaskQueue<item_max_bytes, per_queue_max_items> TaskQueues;
	typedef TaskQueues::RTQ TaskQueue;
	typedef TaskQueue::_Item TaskItem;
private:
	size_t _id;
	MyTaskQueue* queues;
public:
	ServBot(size_t id)
		:	_id(id),
			queues(new MyTaskQueue())
	{ }

	~ServBot()
	{ 
		delete queues;
	}

	TaskQueue* ToDo()
	{
		return queues->ToDo();
	}

	TaskQueue* Done()
	{
		return queues->Done();
	}

	size_t ID()
	{
		return _id;
	}
};
#endif