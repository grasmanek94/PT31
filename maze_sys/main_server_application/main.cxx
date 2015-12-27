#include <unistd.h>
#include <server/Server.hxx>

int main()
{
	Server* network_entity = new Server();

	while (true)
	{
		network_entity->Tick();
		usleep(5000);
	}

	delete network_entity;
	network_entity = NULL;
	return 0;
}
