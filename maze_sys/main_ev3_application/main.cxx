#include <unistd.h>
#include <client/Client.hxx>
#include <ev3/EV3Robot.hxx>
#include <robot_mover/RobotMover.hxx>
#include <iostream>

int main(int argc, char** argv) 
{
	//EV3Robot* robot = new EV3Robot();
	IRobot* robot = NULL;
	Client* network_entity = new Client(robot);

	/*JPS::PathVector path({
		JPS::Pos(300, 0),
		JPS::Pos(30, 15),
		JPS::Pos(30, 30),
		JPS::Pos(30, 15),
		JPS::Pos(15, 0),
		JPS::Pos(0, 0),
		JPS::Pos(5, 0)
	});

	RobotMover::Follow(robot, path);
	while(RobotMover::IsMoving());
	*/

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
