#ifndef SERVBOT_H
#define SERVBOT_H
#include <enet/enetpp.hxx>

#include "RobotTaskQueue.hxx"

class ServBot
{
private:
	size_t _id;
	RobotTaskQueue* queues;
	ENetPeer* peer;
	bool online;
public:
	ServBot(size_t id);
	~ServBot();

	RawQueue* ToDo();
	RawQueue* Done();
	size_t ID() const;

	ENetPeer* GetPeer() const;
	void SetPeer(ENetPeer* peer);

	bool IsOnline();
	void SetOnline(bool isOnline);
};
#endif