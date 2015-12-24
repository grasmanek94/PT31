#include <iostream>
#include <vector>
#include <string>

#include <enet/enetpp.hxx>
#include <networking/PacketInfo.hxx>
#include <networking/PacketData.hxx>
#include "Client.hxx"

void Client::HandleConnect()
{
	printf("connected\n");
	/* Store any relevant client information here. */
	//event.peer->data = (void*)"Client information";
}

void Client::HandleDisonnect()
{
	//printf("%s disconnected.\n", event.peer->data);
	/* Reset the peer's client information. */
	//event.peer->data = NULL;
}

void Client::HandleIdentify(PacketData& data)
{

}

void Client::HandleGotUnknownPacketResponse(PacketData& data)
{
	// okay so we sent a packet to the client and we received it back with "I don't know what this is". What do we do? Nothing?
	// Probably logging for debug purposes and fixing this programatic error...
	// Maybe Assert?
	// Maybe corruption?
}

void Client::HandleUnknownPacket(PacketData& data)
{
	PacketData sendback;

	sendback
		<< SPT_Unknown
		<< SpecifySize{ (uint8_t*)data.Serialize(), data.size() };

	connection->Send(sendback.Serialize(), sendback.size());
}

void Client::HandleReceived(ENetEvent& event)
{
	PacketData data;
	if (connection->GetPacketData(event.packet, data) && data.size())
	{
		uint8_t action;
		data >> action;

		//... then checking what kind of packet we have got
		switch (action)
		{

		case SPT_Identify:
			HandleIdentify(data);
			break;

		case SPT_Unknown:
			HandleGotUnknownPacketResponse(data);
			break;

		case SPT_GotCorruptPacket:
			//HandleGotCorruptPacket(event.peer, data);
			break;

		default:
			HandleUnknownPacket(data);
			break;

		}
	}
}

void Client::TickNetworking()
{
	if (connection->Pull())
	{
		ENetEvent event = connection->Event();

		switch (event.type)
		{

		case ENET_EVENT_TYPE_CONNECT:
			HandleConnect();
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceived(event);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisonnect();
			break;

		case ENET_EVENT_TYPE_NONE:
			//this exists just so gcc will shut up
			break;

		}
	}
}

Client::Client()
	:	connection(new NetworkClient(0x666))
{
	if (connection->GetInitCode()
		|| !connection->Create()
		|| !connection->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	connection->Connect(SERVER_HOST, SERVER_PORT);
}

void Client::Tick()
{
	TickNetworking();
}

Client::~Client()
{
	delete connection;
}