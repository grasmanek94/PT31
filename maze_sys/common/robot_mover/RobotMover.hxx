#ifndef ROBOT_MOVER_H
#define ROBOT_MOVER_H

#include <JumpPointSearch/JPS.hxx>
#include <interfaces/IRobot.hxx>

namespace RobotMover
{
	bool Follow(IRobot* robot, const JPS::PathVector& path);
	bool IsMoving();
};

#endif