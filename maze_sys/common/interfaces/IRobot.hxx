#ifndef HEADER_IRobot_hxx
#define HEADER_IRobot_hxx

#include "ICalibration.hxx"
#include "IDriveControl.hxx"
#include "IGripControl.hxx"
#include "IMecanumControl.hxx"
#include "ILocation.hxx"

class IRobot
{
public:
	virtual IGripControl*		GripControl()	= 0;
	virtual IDriveControl*		DriveControl()	= 0;
	virtual ICalibration*		Calibration()	= 0;

	inline virtual ~IRobot() {};
};

#endif
