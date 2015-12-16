#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <map>
#include <interprocess_position/IPCPos.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include <networking/PacketData.hxx>
#include <enet/enetpp.hxx>
#include "ServBot.hxx"

static const size_t max_bots_for_Server = 2;

class Server
{
public:
	typedef IPCPos<> PosArray;
private:
	PosArray* positions;
	PathProcessorQueue* pathprocessorqueues;
	std::vector<ServBot*> robots;

	IPCQueue* pCalculated;
	IPCQueue* pRequested;

	NetworkServer* connection;

	std::map<ENetPeer*, size_t> PeerToID;

	void TickNetworking();

	void HandleConnect(ENetPeer* peer);
	void HandleDisonnect(ENetPeer* peer);
	void HandleReceived(ENetEvent& event);
	void HandleIdentify(ENetPeer* peer, PacketData& data);
	void HandleGotUnknownPacketResponse(ENetPeer* peer, PacketData& data);
	void HandleUnknownPacket(ENetPeer* peer, PacketData& data);

public:
	Server();
	void Tick();
	~Server();
};
#endif