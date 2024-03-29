#ifndef CLIENT_H
#define CLIENT_H

#include <networking/PacketData.hxx>
#include <networking/NetConfig.hxx>
#include <enet/enetpp.hxx>
#include <interfaces/IRobot.hxx>
#include <robot_mover/RobotMover.hxx>

class Client
{
private:
	NetworkClient* connection;

	void TickNetworking();

	void HandleConnect();
	void HandleDisonnect();
	void HandleReceived(ENetEvent& event);
	void HandleIdentifyResponse(bool acknowledged);
	void HandleGotUnknownPacketResponse(PacketData& data);
	void HandleFollowPath(PacketData& data);
	void HandleUnknownPacket(PacketData& data);

	bool connected;
	size_t serial;
	IRobot* robot;

	unsigned short SERVER_PORT;// = 0x666; // 1638
	std::string SERVER_HOST;// = "127.0.0.1";
public:
	Client(IRobot* robot);
	void Tick();
	~Client();

};
#endif