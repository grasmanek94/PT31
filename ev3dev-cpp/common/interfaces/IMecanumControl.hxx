#ifndef HEADER_IMecanumControl_hxx
#define HEADER_IMecanumControl_hxx

#include "IDriveControl.hxx"

class IMecanumControl : virtual public IDriveControl
{
public:
	virtual void Strafe(int speed, Direction direction, float centimeters) = 0;//move sideways with e.g. Mecanum Wheels
	virtual void MoveTurn(int moveSpeed, Direction moveDirection, float moveCentimeters, int turnSpeed, Direction turnDirection, float turnDegrees) = 0;
};

#endif