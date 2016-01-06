#ifndef HEADER_IDriveControl_hxx
#define HEADER_IDriveControl_hxx

#include "IComponentAvailability.hxx"
#include "ILocation.hxx"

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

	enum ExitCode
	{
		ExitCodeNormal,
		ExitCodeObstruction,
		ExitCodeAborted
	};

	//return ExiCodeAborted if has been aborted by Stop, ExitCodeObstruction if IsObstructed returns true while driving
	//move until obstructed or aborted or completed
	virtual ExitCode Move(int speed, float centimeters) = 0;
	//return ExiCodeAborted if has been aborted by Stop, ExitCodeObstruction may NOT be returned here (how else is a robot supposed to get away from Move() obstruction?)
	virtual ExitCode Turn(int speed, Direction direction, float bias, float degrees) = 0;

	virtual State GetState() const = 0;
	virtual bool IsObstructed() const = 0;

	virtual sPosition GetPos() = 0;
	virtual void SetPos(const sPosition& pos) = 0;
	virtual float GetFrontDistance() const = 0;

	//Make sure this function is thread-safe when implementing
	virtual void Stop() = 0;
};

#endif
