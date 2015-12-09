#ifndef SERVBOT_H
#define SERVBOT_H

#include "RobotTaskQueue.hxx"

class ServBot
{
private:
	size_t _id;
	RobotTaskQueue* queues;
public:
	ServBot(size_t id);
	~ServBot();

	RawQueue* ToDo();
	RawQueue* Done();
	size_t ID() const;
};
#endif