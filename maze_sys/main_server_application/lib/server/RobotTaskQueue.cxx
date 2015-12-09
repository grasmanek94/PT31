#include <interprocess_queue/RawQueue.hxx>
#include "RobotTaskQueue.hxx"

RawQueue* RobotTaskQueue::ToDo()
{
	return &_ToDo;
}

RawQueue* RobotTaskQueue::Done()
{
	return &_Done;
}