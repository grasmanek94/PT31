#include <iostream>
#include <client/Client.hxx>

int main(int argc, char** argv) {
	std::cout << "Starting" << std::endl;
	Client* client = new Client();
	std::cout << "Created client" << std::endl;
		
	std::cout << "Start receiving..." << std::endl;
	while(true) 
	{
		client->Tick();
		//client->Send("Hoi! 1234567890");
	}
	
	return 0;
}
