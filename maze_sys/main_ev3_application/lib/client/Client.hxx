#ifndef CLIENT_H
#define CLIENT_H

#include <networking/PacketData.hxx>
#include <networking/NetConfig.hxx>
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

	bool connected;
	size_t id;
public:
	Client();
	void Tick();
	~Client();

};
#endif