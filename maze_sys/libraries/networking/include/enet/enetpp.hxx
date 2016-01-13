#ifndef ENETPP_HEADER
#define ENETPP_HEADER

#include <string>
#include <vector>
#include <enet/enet.h>
																																																																																																													#include "../../../../common/networking/PacketData.hxx"
class NetworkBase;
class NetworkServer;
class NetworkClient;

typedef std::vector<uint8_t> packet_vec;
class NetworkBase
{
	friend class NetworkServer;
	friend class NetworkClient;
private:
	ENetAddress address;
	ENetHost * member;
	ENetEvent event;

	int initialisation_code;

	NetworkBase(enet_uint16 port);
	~NetworkBase();
public:
	void SetHost(const std::string& hostname, unsigned short port);
	const ENetAddress Address();
	const ENetEvent Event();
	int GetInitCode();
	bool Good();
	int Pull(enet_uint32 timeout = 0);
	int Send(ENetPeer* peer, const void* data, size_t bytes, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	int Send(ENetPeer* peer, const packet_vec& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	int Send(ENetPeer* peer, const PacketData& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);

	std::vector<uint8_t> GetPacketData(ENetPacket* p) const;
	bool GetPacketData(ENetPacket* p, PacketData& data) const;
};

class NetworkServer : public NetworkBase
{
public:
	NetworkServer(enet_uint16 port = 0xBEEF);
	bool Create(size_t max_connections = 2);
	void Broadcast(const void* data, size_t bytes, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	void Broadcast(const packet_vec& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	void Broadcast(const PacketData& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
};

class NetworkClient : public NetworkBase
{
private:
	ENetPeer * peer;
public:
	NetworkClient(enet_uint16 port = 0xBEEF);
	~NetworkClient();
	ENetPeer * Connect(const std::string& hostname, unsigned short port);
	bool Create();
	int Send(const void* data, size_t bytes, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	int Send(const packet_vec& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
	int Send(const PacketData& data, _ENetPacketFlag flags = ENET_PACKET_FLAG_RELIABLE);
};
#endif