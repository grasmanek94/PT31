#include <iostream>
#include <vector>
#include <string>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include "ServBot.hxx"

#include <enet/enetpp.hxx>
#include <networking/PacketInfo.hxx>
#include <networking/PacketData.hxx>
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

void Server::HandleIdentify(ENetPeer* peer, PacketData& data)
{
	if (data.remaining_size())
	{
		uint8_t id;
		data >> id;
		if (id < robots.size())
		{
			//this is a bot
			connection->Send(peer, packet_vec{ SPT_IdentifyAcknowledged });
		}
		else
		{
			//this is not a bot
			connection->Send(peer, packet_vec{ SPT_IdentifyDenied });
		}
	}
	else
	{
		connection->Send(peer, packet_vec{ SPT_GotCorruptPacket, SPT_Identify });
	}
}

void Server::HandleGotUnknownPacketResponse(ENetPeer* peer, PacketData& data)
{
	// okay so we sent a packet to the client and we received it back with "I don't know what this is". What do we do? Nothing?
	// Probably logging for debug purposes and fixing this programatic error...
	// Maybe Assert?
	// Maybe corruption?
}

void Server::HandleUnknownPacket(ENetPeer* peer, PacketData& data)
{
	PacketData sendback;
	uint8_t type = SPT_Unknown;
	SpecifySize senddata({ (uint8_t*)data.Serialize(), data.size() });
	sendback
		<< type
		<< senddata;
	connection->Send(peer, sendback.Serialize(), sendback.size());
}

void Server::HandleReceived(ENetEvent& event)
{
	PacketData data;
	data.Deserialize(event.packet->data, event.packet->dataLength);
	enet_packet_destroy(event.packet);

	if (data.size())
	{
		uint8_t action;
		data >> action;

		//... then checking what kind of packet we have got
		switch (action)
		{

		case SPT_Identify:
			HandleIdentify(event.peer, data);
			break;

		case SPT_Unknown:
			HandleGotUnknownPacketResponse(event.peer, data);
			break;

		case SPT_GotCorruptPacket:
			//HandleGotCorruptPacket(event.peer, data);
			break;

		default:
			HandleUnknownPacket(event.peer, data);
			break;

		}
	}
}

void Server::TickNetworking()
{
	if (connection->Pull())
	{
		ENetEvent event = connection->Event();

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
		connection(new NetworkServer(0x666))
{
	for (size_t i = 0; i < max_bots_for_Server; ++i)
	{
		robots.push_back(new ServBot(i));
	}

	pCalculated = pathprocessorqueues->Calculated();
	pRequested = pathprocessorqueues->Request();

	if (connection->GetInitCode()
		|| !connection->Create(8)
		|| !connection->Good())
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
	delete connection;
}