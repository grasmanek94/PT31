#include <iostream>
#include <client/Client.hxx>

int main(int argc, char** argv) {
	std::cout << "Starting" << std::endl;
	NetClient* client = new NetClient("10.0.0.1", 0x666);
	std::cout << "Created client" << std::endl;
	
	
	Packet data;
	if (argc <= 1)
	{
		data.type = 0;
		data.data = "Hoi!";
	}
	else
	{
		data.type=0;
		data.data=argv[1];
		
	}
	if (!client->Send(data))
	{
		std::cout << "Maybe sent: " << data.data << std::endl;
	} else
	{
		std::cout << "Nope!" << std::endl;
	}
	
	std::cout << "Start receiving..." << std::endl;
	while(true) {
		client->Run();
	}
	
	return 0;
}
