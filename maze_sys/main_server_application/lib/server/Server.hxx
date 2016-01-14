#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <map>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include <networking/PacketData.hxx>
#include <networking/NetConfig.hxx>
#include <enet/enetpp.hxx>
#include "ServBot.hxx"

#include "CommandParamsParser.hxx"
#define DEFINE_COMMAND(command_name) void command_ ## command_name(const std::string& params, ParseInput& parser)

static const size_t max_bots_for_Server = 2;
class Server
{
public:
	typedef IPCPos<> PosArray;
private:
	PosArray* positions;
	PathProcessorQueue* pathprocessorqueues;
	std::vector<ServBot*> robots;
	std::string console_buffer;

	IPCQueue* pathCalculated;
	IPCQueue* pathRequested;

	NetworkServer* connection;

	//well no 'auto' allowed so at leas we can typedef std::map<ENetPeer*, size_t>::iterator
	typedef std::map<ENetPeer*, size_t> P2ID;
	typedef P2ID::iterator P2IDIt;

	P2ID PeerToID;

	void TickNetworking();
	void TickTasking();
	void TickConsole();

	void HandleConnect(ENetPeer* peer);
	void HandleDisonnect(ENetPeer* peer);
	void HandleReceived(ENetEvent& event);
	void HandleIdentify(ENetPeer* peer, PacketData& data);
	void HandleGotUnknownPacketResponse(ENetPeer* peer, PacketData& data);
	void HandleUnknownPacket(ENetPeer* peer, PacketData& data);
	void HandleConsoleCommand(const std::string& command);

	//Commands:
	void RegisterCommands();
	DEFINE_COMMAND(exit);
	DEFINE_COMMAND(robot_move_to);
public:
	Server();
	void Tick();
	~Server();
};

//ServerCommandHandler
namespace CommandHandler
{
	class Command
	{
	public:
		typedef void (Server::*fn)(const std::string& params, ParseInput& parser);
		ParseInput parser;
	private:
		fn function;
	public:
		Command(fn function);
		void do_command(Server* context, const std::string& params);
	};

	bool Execute(Server* context, const std::string& cmdtext);
	void Register(Command::fn handler, const std::string& command_name, const std::string& format);
};

#define IMPLEMENT_COMMAND(command_name) void Server::command_ ## command_name(const std::string& params, ParseInput& parser)
#define REGISTER_COMMAND(command_name, format) CommandHandler::Register(&Server::command_ ## command_name, #command_name, format)
#endif