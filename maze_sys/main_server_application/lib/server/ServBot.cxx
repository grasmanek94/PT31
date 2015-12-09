#include "RobotTaskQueue.hxx"
#include "ServBot.hxx"

ServBot::ServBot(size_t id)
	:	_id(id),
		queues(new RobotTaskQueue())
{ }

ServBot::~ServBot()
{ 
	delete queues;
}

RawQueue* ServBot::ToDo()
{
	return queues->ToDo();
}

RawQueue* ServBot::Done()
{
	return queues->Done();
}

size_t ServBot::ID() const
{
	return _id;
}

ENetPeer* ServBot::GetPeer() const
{
	return peer;
}

void ServBot::SetPeer(ENetPeer* peer)
{
	this->peer = peer;
}
