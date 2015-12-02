#include "EV3Movements.hxx"
#include <math.h>       /* atan2 */
#define PI 3.14159265

Movements::Movements(EV3DriveControl* RobotDriveControl)
:RobotDriveControl(RobotDriveControl)
{};

void Movements::Pathdrive(int speed, int direction, int bias, int degrees, int distance)
{
		RobotDriveControl->Turn(speed, (IDriveControl::Direction)direction, bias, degrees);
		RobotDriveControl->Move(speed, distance);
}

void Movements::CalcValues(JPS::PathVector positions)
{
	for (int i = 0; i < positions.size() - 1; i++)
	{
		JPS::Position positionA = positions[i];
		JPS::Position positionB = positions[i++];

		int speed = 50;
		double degrees = atan2(positionB.y - positionA.y, positionB.x - positionA.x) * 180 / PI;
		double bias = 0.5;
		double distance = sqrt(pow(positionB.y - positionA.y, 2) + pow(positionB.x - positionA.x, 2));

		Pathdrive(speed, IDriveControl::DirectionRight, bias, degrees, distance);
	}
}