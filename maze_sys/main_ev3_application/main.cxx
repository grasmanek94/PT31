#include <unistd.h>
#include <client/Client.hxx>

int main(int argc, char** argv) 
{
	Client* network_entity = new Client();
	while(true) 
	{
		network_entity->Tick();
		usleep(5000);
	}
	
	return 0;
}
