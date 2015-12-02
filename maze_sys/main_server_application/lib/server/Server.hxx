#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

template <size_t max_bots = 2, size_t item_max_bytes = 2048, size_t per_queue_max_items = 128>
class Server
{
public:
	typedef ServBot<item_max_bytes, per_queue_max_items> ServerBot;
	typedef PathProcessorQueue<> PPQ;
	typedef IPCPos<> PosArray;
private:
	PosArray* positions;
	PPQ* pathprocessorqueues;
	std::vector<ServerBot> robots;

	PPQ::MyIPCQueue* pCalculated;
	PPQ::MyIPCQueue* pRequested;

public:
	Server()
		:	positions(new PosArray()),
			pathprocessorqueues(new PPQ())
	{
		for (size_t i = 0; i < max_bots; ++i)
		{
			robots.push_back(ServerBot(i));
		}

		pCalculated = pathprocessorqueues->Calculated();
		pRequested = pathprocessorqueues->Request();
	}

	void Run()
	{
		// OperationIdentifier == BotID
		if (pCalculated->Count())
		{
			PPQ::MyQueueItem item;
			if (pCalculated->Pop(&item))
			{
				robots[item.GetOperationIdentifier()].ToDo()->Push(&item);
			}
		}
	}

	~Server()
	{
		delete positions;
		delete pathprocessorqueues;
	}
};
#endif