#include <iostream>
#include <client/Client.hxx>

int main() {
	std::cout << "Starting" << std::endl;
	NetClient* client = new NetClient("10.0.0.1", 0x666);
	std::cout << "Created client" << std::endl;
	
	Packet data;
	data.type = 0;
	data.data = "Hoi!";
	client->Send(data);
	std::cout << "Maybe sent: " << data.data << std::endl;
	return 0;
}
