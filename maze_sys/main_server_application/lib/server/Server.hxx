#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

#include <enet/enetpp.hxx>

static const size_t max_bots_for_Server = 2;

class Server
{
public:
	typedef IPCPos<> PosArray;
private:
	PosArray* positions;
	PathProcessorQueue* pathprocessorqueues;
	std::vector<ServBot> robots;

	IPCQueue* pCalculated;
	IPCQueue* pRequested;

	NetworkServer* ns;

	void TickNetworking();
public:
	Server();
	void Tick();
	~Server();
};
#endif