#include <iostream>
#include <vector>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

#include <enet/enetpp.hxx>

#include "Server.hxx"

void Server::TickNetworking()
{
	if (ns->Pull())
	{
		std::cout << "ns->Event()" << std::endl;
		ENetEvent event = ns->Event();
		std::vector<uint8_t> data_vec;

		std::cout << "switch()" << std::endl;
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			std::cout << "ENET_EVENT_TYPE_CONNECT" << std::endl;
			printf("A new client connected from %x:%u.\n",
				event.peer->address.host,
				event.peer->address.port);
			/* Store any relevant client information here. */
			//event.peer->data = (void*)"Client information";
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			std::cout << "ENET_EVENT_TYPE_RECEIVE" << std::endl;
			data_vec = ns->GetPacketData(event.packet);
			data_vec.push_back(0);
			printf("%s", data_vec.data());
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "ENET_EVENT_TYPE_DISCONNECT" << std::endl;
			//printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			//event.peer->data = NULL;
			break;

		case ENET_EVENT_TYPE_NONE:
			std::cout << "ENET_EVENT_TYPE_NONE" << std::endl;
			//well this shouldn't happen
			break;
		}
	}
}

Server::Server()
	:	positions(new PosArray()),
		pathprocessorqueues(new PathProcessorQueue()),
		ns(new NetworkServer(0x666))
{
	for (size_t i = 0; i < max_bots_for_Server; ++i)
	{
		robots.push_back(new ServBot(i));
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


void Server::Tick()
{
	// OperationIdentifier == BotID
	if (pCalculated->Count())
	{
		QueueItem item;
		if (pCalculated->Pop(&item))
		{
			robots[item.GetOperationIdentifier()]->ToDo()->Push(&item);
		}
	}

	TickNetworking();
}

Server::~Server()
{
	for (size_t i = 0; i < robots.size(); ++i)
	{
		delete robots[i];
		robots[i] = NULL;
	}

	delete positions;
	delete pathprocessorqueues;
	delete ns;
}