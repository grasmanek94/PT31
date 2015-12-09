#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

#include <enet/enetpp.hxx>

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

	NetworkServer* ns;

	void RunNetworking()
	{
		if (ns->Pull())
		{
			ENetEvent event = ns->Event();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("%s\n", event.packet->data);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				break;
			}
		}
	}
public:
	Server()
		:	positions(new PosArray()),
			pathprocessorqueues(new PPQ()),
			ns(new NetworkServer())
	{
		for (size_t i = 0; i < max_bots; ++i)
		{
			robots.push_back(ServerBot(i));
		}

		pCalculated = pathprocessorqueues->Calculated();
		pRequested = pathprocessorqueues->Request();

		if (ns->GetInitCode()
			|| !ns->Create(8)
			|| !ns->Good())
		{
			std::cout << "An error occurred while trying to create an ENet object." << std::endl;
			exit(EXIT_FAILURE);
		}
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

		RunNetworking();
	}

	~Server()
	{
		delete positions;
		delete pathprocessorqueues;
		delete ns;
	}
};
#endif