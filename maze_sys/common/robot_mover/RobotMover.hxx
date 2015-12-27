#ifndef ROBOT_MOVER_H
#define ROBOT_MOVER_H

#include <JumpPointSearch/JPS.hxx>
#include <interfaces/IRobot.hxx>

class RobotMover
{
private:
	//pthread_t thread;
public:
	RobotMover(IRobot* _robot);
	~RobotMover();

	bool RobotGoTo(JPS::PathVector* path);
	bool IsMoving();
};


#endif