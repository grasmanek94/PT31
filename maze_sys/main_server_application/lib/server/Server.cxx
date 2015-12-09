#include <iostream>
#include <vector>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

#include <enet/enetpp.hxx>
#include <networking/PacketInfo.hxx>

#include "Server.hxx"

void Server::HandleConnect(ENetPeer* peer)
{
	//printf("A new client connected from %x:%u.\n",
	//	event.peer->address.host,
	//	event.peer->address.port);
	/* Store any relevant client information here. */
	//event.peer->data = (void*)"Client information";
	PeerToID[peer] = -1;
}

void Server::HandleDisonnect(ENetPeer* peer)
{
	//printf("%s disconnected.\n", event.peer->data);
	/* Reset the peer's client information. */
	//event.peer->data = NULL;
	PeerToID.erase(peer);
}

void Server::HandleIdentify(ENetPeer* peer, packet_vec& data)
{
	if (data.size())
	{
		uint8_t id = data[0];
		if (id >= robots.size())
		{

		}
		else
		{

		}
	}
	else
	{
		ns->Send(peer, packet_vec{ SPT_GotCorruptPacket, SPT_Identify });
	}
}

void Server::HandleGotUnknownPacketResponse(ENetPeer* peer, packet_vec& data)
{
	// okay so we sent a packet to the client and we received it back with "I don't know what this is". What do we do? Nothing?
	// Probably logging for debug purposes and fixing this programatic error...
	// Maybe Assert?
	// Maybe corruption?
}

void Server::HandleUnknownPacket(ENetPeer* peer, packet_vec& data)
{
	data.insert(data.begin(), SPT_Unknown);
	ns->Send(peer, data);
}

void Server::HandleReceived(ENetEvent& event)
{
	std::vector<uint8_t> data_vec;
	data_vec = ns->GetPacketData(event.packet);

	if (data_vec.size())
	{
		uint8_t action = data_vec[0];

		switch (action)
		{

		case SPT_Identify:
			data_vec.erase(data_vec.begin());
			HandleIdentify(event.peer, data_vec);
			break;

		case SPT_Unknown:
			data_vec.erase(data_vec.begin());
			HandleGotUnknownPacketResponse(event.peer, data_vec);
			break;

		default:
			HandleUnknownPacket(event.peer, data_vec);
			break;

		}
	}
}

void Server::TickNetworking()
{
	if (ns->Pull())
	{
		ENetEvent event = ns->Event();

		switch (event.type)
		{

		case ENET_EVENT_TYPE_CONNECT:
			HandleConnect(event.peer);
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceived(event);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisonnect(event.peer);
			break;

		case ENET_EVENT_TYPE_NONE:
			//this exists just so gcc will shut up
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