#include <iostream>
#include <vector>
#include <string>

#include <ini/INI.h>
#include <enet/enetpp.hxx>
#include <networking/PacketInfo.hxx>
#include <networking/PacketData.hxx>
#include "Client.hxx"

unsigned short SERVER_PORT = 0x666; // 1638
std::string SERVER_HOST = "127.0.0.1";

void Client::HandleConnect()
{
	connected = true;
	std::cout << "HandleConnect" << std::endl;
	/* Store any relevant client information here. */
	//event.peer->data = (void*)"Client information";
	PacketData identifyPacket;
	identifyPacket
		<< SPT_Identify
		<< serial;
	connection->Send(identifyPacket);
}

void Client::HandleDisonnect()
{
	connected = false;
	std::cout << "HandleDisonnect" << std::endl;
	//printf("%s disconnected.\n", event.peer->data);
	/* Reset the peer's client information. */
	//event.peer->data = NULL;
	connection->Connect(SERVER_HOST, SERVER_PORT);
}

void Client::HandleIdentifyResponse(bool acknowledged)
{
	std::cout << "HandleIdentifyResponse" << std::endl;
	if (!acknowledged)
	{
		//show error on ev3 LCD?
		std::cout << "!acknowledged" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "I AM ACKNOWLEDGED" << std::endl;
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
		<< data;

	connection->Send(sendback);
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

		case SPT_IdentifyAcknowledged:
		case SPT_IdentifyDenied:
			HandleIdentifyResponse(action == SPT_IdentifyAcknowledged);
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
	:	connection(new NetworkClient(0x666)),
		connected(false),
		serial(0)
{
	if (connection->GetInitCode()
		|| !connection->Create()
		|| !connection->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	INI<> config_file("config.ini", true);
	//get section key default_value
	SERVER_HOST = config_file.get("network", "host", std::string("127.0.0.1"));
	SERVER_PORT = config_file.get("network", "port", 0x666);

	serial = config_file.get("robot", "serial", 0);

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