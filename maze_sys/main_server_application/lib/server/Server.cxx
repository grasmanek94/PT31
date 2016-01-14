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

Server::Server()
	: positions(new PosArray()),
	pathprocessorqueues(new PathProcessorQueue()),
	connection(new NetworkServer(0x666))
{
	for (size_t i = 0; i < max_bots_for_Server; ++i)
	{
		robots.push_back(new ServBot(i));
	}

	pathCalculated = pathprocessorqueues->Calculated();
	pathRequested = pathprocessorqueues->Request();

	if (connection->GetInitCode()
		|| !connection->Create(8)
		|| !connection->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	RegisterCommands();
}

void Server::HandleConnect(ENetPeer* peer)
{
	std::cout 
		<< "A new client connected from "
		<< std::hex << peer->address.host << std::dec
		<< ":"
		<< peer->address.port
		<< std::endl;
	/* Store any relevant client information here. */
	//event.peer->data = (void*)"Client information";
	PeerToID[peer] = INVALID_ROBOT_ID;
}

void Server::HandleDisonnect(ENetPeer* peer)
{
	std::cout
		<< "A client diconnected "
		<< std::hex << peer->address.host << std::dec
		<< ":"
		<< peer->address.port
		<< std::endl;
	/* Reset the peer's client information. */
	//event.peer->data = NULL;
	P2IDIt it = PeerToID.find(peer);
	if (it->second != INVALID_ROBOT_ID)
	{
		robots[it->second]->SetOnline(false);
		robots[it->second]->SetPeer(NULL);
	}
	PeerToID.erase(it);
}

void Server::HandleIdentify(ENetPeer* peer, PacketData& data)
{
	std::cout 
		<< "Received Identify request from " 
		<< std::hex << peer->address.host << std::dec
		<< ":"
		<< peer->address.port 
		<< std::endl;

	if (data.remaining_size() == sizeof(size_t))
	{
		size_t serial;
		data >> serial;

		std::cout << "Using serial id " << serial << std::endl;
		//guaranteed to be found because in Connect peer is added
		P2IDIt it = PeerToID.find(peer);
			
		if (serial < robots.size())
		{
			std::cout << "Serial has valid range" << std::endl;
			if (!robots[serial]->IsOnline() && it->second == INVALID_ROBOT_ID)
			{
				robots[serial]->SetOnline(true);
				robots[serial]->SetPeer(peer);

				it->second = serial;

				std::cout << "id: " << serial << " OK" << std::endl;
				//this is a bot
				connection->Send(peer, packet_vec{ SPT_IdentifyAcknowledged });
				return;
			}
			//if robot sends Identify again while acknowledged just let it pass
			else if (it->second == serial)
			{
				connection->Send(peer, packet_vec{ SPT_IdentifyAcknowledged });
				return;
			}
		}
	}
	std::cout << "FAIL " << data.remaining_size() << "/" << data.size() << std::endl;
	//this is not a bot or correct identification (e.g. someone tries to use existing serial number)
	connection->Send(peer, packet_vec{ SPT_IdentifyDenied });
}

void Server::HandleGotUnknownPacketResponse(ENetPeer* peer, PacketData& data)
{
	std::cout << "HandleGotUnknownPacketResponse" << std::endl;
	// okay so we sent a packet to the client and we received it back with "I don't know what this is". What do we do? Nothing?
	// Probably logging for debug purposes and fixing this programatic error...
	// Maybe Assert?
	// Maybe corruption?
}

void Server::HandleUnknownPacket(ENetPeer* peer, PacketData& data)
{
	std::cout << "HandleUnknownPacket" << std::endl;
	PacketData sendback;

	sendback
		<< SPT_Unknown
		<< data;

	connection->Send(peer, sendback);
}

void Server::HandleReceived(ENetEvent& event)
{
	PacketData data;
	
	if (connection->GetPacketData(event.packet, data) && data.size())
	{
		ServerPacketType action;
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

void Server::HandleConsoleCommand(const std::string& command)
{
	CommandHandler::Execute(this, command);
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

void Server::TickTasking()
{
	// OperationIdentifier == BotID
	if (pathCalculated->Count())
	{
		QueueItem item;
		if (pathCalculated->Pop(&item))
		{
			robots[item.GetOperationIdentifier()]->ToDo()->Push(&item);
		}
	}
}

void Server::Tick()
{
	TickTasking();
	TickNetworking();
	TickConsole();
}

#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

bool lineAvailable()
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
	return (FD_ISSET(0, &fds));
}

void Server::TickConsole()
{
	if (lineAvailable())
	{
		std::getline(std::cin, console_buffer);
		if (console_buffer.size())
		{
			HandleConsoleCommand(console_buffer);
			console_buffer.clear();
		}
	}
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

//commands

void Server::RegisterCommands()
{
	REGISTER_COMMAND(exit, "");
	REGISTER_COMMAND(robot_move_to, "iff");
}

IMPLEMENT_COMMAND(exit)
{
	std::cout << "you requested the application to exit but unfortunately we don't allow this (cheat tip: try ctrl+c)" << std::endl;
}

IMPLEMENT_COMMAND(robot_move_to)
{
	if (parser.Good() != 3)
	{
		std::cout << "Usage: robot_move_to <robot id> <to x> <to y>" << std::endl;
		return;
	}

	int robot_id = parser.GetNext<long>();
	float move_x = parser.GetNext<float>();
	float move_y = parser.GetNext<float>();

	std::cout << "Moving robot with id " << robot_id << " to pos {" << move_x << ", " << move_y << "}" << std::endl;
}
