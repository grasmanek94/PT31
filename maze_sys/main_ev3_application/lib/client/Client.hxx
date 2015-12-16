#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <networking/PacketData.hxx>
#include <enet/enetpp.hxx>

class Client
{
private:
	NetworkClient* connection;

	void TickNetworking();

	void HandleConnect();
	void HandleDisonnect();
	void HandleReceived(ENetEvent& event);
	void HandleIdentify(PacketData& data);
	void HandleGotUnknownPacketResponse(PacketData& data);
	void HandleUnknownPacket(PacketData& data);

public:
	Client();
	void Tick();
	~Client();
};
#endif