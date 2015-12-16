#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enetpp.hxx>

class NetClient
{
	private:
		NetworkClient* client;
		void RunNetworking()
		{
			if (client->Pull())
			{
				ENetEvent event = client->Event();
				std::vector<uint8_t> data_vec;
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
					data_vec = client->GetPacketData(event.packet);
					data_vec.push_back(0);
					printf("%s", data_vec.data());
					
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					//printf("%s disconnected.\n", event.peer->data);
					/* Reset the peer's client information. */
					//event.peer->data = NULL;
					break;

				case ENET_EVENT_TYPE_NONE:
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
		
		void Run()
		{
			RunNetworking();
		}
		
		int Send(const std::string& data)
		{
			return client->Send(data.c_str(), data.size());
		}

		~NetClient()
		{
			delete client;
		}
};
#endif
