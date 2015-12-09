#include <iostream>
#include <client/Client.hxx>

int main(int argc, char** argv) {
	std::cout << "Starting" << std::endl;
	NetClient* client = new NetClient("127.0.0.1", 0x666);
	std::cout << "Created client" << std::endl;
		
	std::cout << "Start receiving..." << std::endl;
	while(true) 
	{
		client->Run();
		client->Send("Hoi! 1234567890");
	}
	
	return 0;
}
