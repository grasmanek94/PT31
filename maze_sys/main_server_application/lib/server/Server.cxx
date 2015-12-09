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
		ENetEvent event = ns->Event();
		std::vector<uint8_t> data_vec;
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected from %x:%u.\n",
				event.peer->address.host,
				event.peer->address.port);
			/* Store any relevant client information here. */
			//event.peer->data = (void*)"Client information";
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			data_vec = ns->GetPacketData(event.packet);
			data_vec.push_back(0);
			printf("%s", data_vec.data());
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			//printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			//event.peer->data = NULL;
			break;

		case ENET_EVENT_TYPE_NONE:
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
		robots.push_back(ServBot(i));
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
			robots[item.GetOperationIdentifier()].ToDo()->Push(&item);
		}
	}

	TickNetworking();
}

Server::~Server()
{
	delete positions;
	delete pathprocessorqueues;
	delete ns;
}