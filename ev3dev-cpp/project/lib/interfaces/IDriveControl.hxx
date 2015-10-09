#ifndef HEADER_IDriveControl_hxx
#define HEADER_IDriveControl_hxx

class IDriveControl
{
public:
	enum State
	{
		StateUnknown,
		StateError,
		StateSuccess,
		StateNotConnected,

		StateStopped,
		StateMoving,
		StateObstacleEncountered,
		StateTurning
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
	virtual bool Available() const = 0;
};

#endif