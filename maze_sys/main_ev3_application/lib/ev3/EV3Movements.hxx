#include "EV3Robot.hxx"
#include <JumpPointSearch/JPS.hxx>

class Movements
{
	private:	
		EV3DriveControl* RobotDriveControl;

	public:
		Movements(EV3DriveControl* RobotDriveControl);
		void Pathdrive(int speed, int direction, int bias, int degrees, int distance);
		void CalcValues(JPS::PathVector positions);
};