#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <enet/enetpp.hxx>
#include "Robot.h"

class NetServer
{
	private:
		vector<Robot> robotList;
		
		
	public:
		NetServer();
		ENetEvent Receive();
		bool Send(Robot robot, QueueItem item);
};

#endif
