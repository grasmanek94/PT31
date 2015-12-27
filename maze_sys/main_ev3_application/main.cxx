#include <unistd.h>
#include <client/Client.hxx>
#include <ev3/EV3Robot.hxx>

int main(int argc, char** argv) 
{
	//EV3Robot* robot = new EV3Robot();
	IRobot* robot = NULL;
	Client* network_entity = new Client(robot);

	while(true) 
	{
		network_entity->Tick();
		usleep(5000);
	}
	
	delete network_entity;
	network_entity = NULL;
	delete robot;
	robot = NULL;

	return 0;
}
