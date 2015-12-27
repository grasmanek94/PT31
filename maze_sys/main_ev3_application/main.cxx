#include <unistd.h>
#include <client/Client.hxx>
#include <ev3/EV3Robot.hxx>

int main(int argc, char** argv) 
{
	IRobot* robot = new EV3Robot();
	Client* network_entity = new Client(robot);

	while(true) 
	{
		network_entity->Tick();
		usleep(5000);
	}
	
	delete network_entity;
	delete robot;

	return 0;
}
