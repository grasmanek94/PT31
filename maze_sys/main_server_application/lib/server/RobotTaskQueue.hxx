#ifndef ROBOTTASKQUEUE_H
#define ROBOTTASKQUEUE_H

#include <interprocess_queue/RawQueue.hxx>

class RobotTaskQueue
{
private:

	RawQueue _ToDo;
	RawQueue _Done;

public:

	RobotTaskQueue();
	~RobotTaskQueue();

	RawQueue* ToDo();
	RawQueue* Done();
};
#endif