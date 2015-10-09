#ifndef HEADER_IRobot_hxx
#define HEADER_IRobot_hxx

#include "ICalibration.hxx"
#include "IDriveControl.hxx"
#include "IGripControl.hxx"
#include "IMecanumControl.hxx"
#include "ISensing.hxx"

class IRobot
{
public:
	virtual IGripControl*		GripControl()	= 0;
	virtual IDriveControl*		DriveControl()	= 0;
	virtual ISensing*			Measure()		= 0;
};

#endif