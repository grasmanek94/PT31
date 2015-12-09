#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enetpp.hxx>

struct Packet {
	uint8_t type;
	std::string data;
} ;

//template < std::string serverAddress = "10.0.0.1", uint32_t serverPort = 0x666 >
class NetClient
{
	public:
		
	
	private:
		NetworkClient* client;
		void RunNetworking()
		{
			if (client->Pull())
			{
				ENetEvent event = client->Event();
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					printf("connected to %x:%u.\n",
						event.peer->address.host,
						event.peer->address.port);
					/* Store any relevant client information here. */
					//event.peer->data = "Client information";
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					printf("%s\n", event.packet->data);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);

					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%s disconnected.\n", event.peer->data);
					/* Reset the peer's client information. */
					event.peer->data = NULL;
					break;
				}
			}
		}
			
	public:
		NetClient(std::string serverAddress, uint32_t serverPort)
			:	client(new NetworkClient())
			
		{
			if (	client->GetInitCode() 
				||	!client->Create()
				||	!client->Good()
				||	!client->Connect(serverAddress, serverPort))
			{
				std::cout << "An error occurred while trying to create an ENet object." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/* AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH! */
		void Run()
		{
			/*if (pCalculated->Count())
			{
				PPQ::MyQueueItem item;
				if (pCalculated->Pop(&item))
				{
					robots[item.GetOperationIdentifier()].ToDo()->Push(&item);
				}
			}*/

			RunNetworking();
		}
		
		void Send(Packet data)
		{
			client->Send(&data, sizeof(data));
		}

		~NetClient()
		{
			delete client;
		}
};
#endif
