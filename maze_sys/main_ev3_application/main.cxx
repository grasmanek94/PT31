#include <string>
#include <iostream>
#include <client/Client.hxx>

int main(int argc, char** argv) {
	std::cout << "Starting" << std::endl;
	//NetClient* client = new NetClient("127.0.0.1", 0x666);
	
	NetworkClient* client = new NetworkClient();
	if (	client->GetInitCode() 
		||	!client->Create()
		||	!client->Good()
		||	!client->Connect("192.168.137.51", 0x4242))
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Created client" << std::endl;
	std::cout << "Sending crap" << std::endl;
	
	
	int outerTimes = 10;
	int innerTimes = 100;
	
	//struct timeval t1, t2;
	struct timespec t1, t2;
	unsigned long avgTotal = 0;
	
	for (int tt = 0; tt < outerTimes; tt++)
	{
		unsigned long elapsedTime = 0;
		
		int t = 0;
		while( t < innerTimes )
		{
			std::string data = "hoi";
			std::string expected = "woah";
			//gettimeofday(&t1, NULL);
			clock_gettime(CLOCK_REALTIME, &t1);
			client->Send(data.c_str(), data.size());
			
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
					//printf("%s", data_vec.data());
					//if (expected.compare(data_vec.data()) == 0) {
						//gettimeofday(&t2, NULL);
						clock_gettime(CLOCK_REALTIME, &t2);
						elapsedTime += (t2.tv_nsec - t1.tv_nsec);
						t++;
					//}
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
		std::cout << elapsedTime/innerTimes << std::endl;
		
		avgTotal+=elapsedTime/innerTimes;
	}
	std::cout << "10x everything: " << avgTotal/outerTimes  << " ns" << std::endl;

	
	return 0;
}
