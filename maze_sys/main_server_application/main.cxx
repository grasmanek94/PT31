#include <iostream>
#include <chrono>
#include <thread>
#include <enet/enetpp.hxx>
#include <ctime>
#include <sys/time.h>

int main()
{
	std::cout << "Starting" << std::endl;
	NetworkServer* server = new NetworkServer(0x4242);
	std::string data2send = "woah";

	
	if (	server->GetInitCode() 
		||	!server->Create(1024) 
		||	!server->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	

	while (true)
	{
		if (server->Pull())
		{
			ENetEvent event = server->Event();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = (void*)"Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				//printf("%s\n", event.packet->data);
				/* Clean up the packet now that we're done using it. */
				server->Send(event.peer, data2send.c_str(), data2send.size());
				enet_packet_destroy(event.packet);
				
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", (char*)event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				break;
			case ENET_EVENT_TYPE_NONE:
				break;
			}
		}
	}
	delete server;

	return 0;
}
