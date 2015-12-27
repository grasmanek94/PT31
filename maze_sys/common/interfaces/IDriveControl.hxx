#ifndef HEADER_IDriveControl_hxx
#define HEADER_IDriveControl_hxx

#include "IComponentAvailability.hxx"

class IDriveControl : public virtual IComponentAvailability
{
public:
	enum State
	{
		StateStopped,
		StateMoving
	};

	enum Direction
	{
		DirectionLeft,
		DirectionRight
	};

public:

	virtual void Move(int speed, float centimeters) = 0;
	virtual void Turn(int speed, Direction direction, float bias, float degrees) = 0;

	virtual State GetState() const = 0;

	//Make sure this function is thread-safe when implementing
	virtual void Stop() = 0;
};

#endif
