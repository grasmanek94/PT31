#ifndef HEADER_EV3Robot_hxx
#define HEADER_EV3Robot_hxx

#include <interfaces/IRobot.hxx>
#include <ev3dev/ev3dev.hxx>

#include "EV3Calibration.hxx"
#include "EV3DriveControl.hxx"
#include "EV3GripControl.hxx"
#include "EV3Sensing.hxx"

class EV3Robot : public IRobot
{
private:
	EV3GripControl _GripControl;
	EV3DriveControl _DriveControl;
	EV3Sensing _Measure;
	EV3Calibration _calibration;

public:
	EV3Robot(
		ev3dev::port_type _grip_motor = ev3dev::OUTPUT_B,
		ev3dev::port_type _grip_sensor = ev3dev::INPUT_1,
		ev3dev::port_type left_motor = ev3dev::OUTPUT_A,
		ev3dev::port_type right_motor = ev3dev::OUTPUT_D,
		ev3dev::port_type obstruction_sensor = ev3dev::INPUT_2,
		ev3dev::port_type distance_sensor = ev3dev::INPUT_4,
		ev3dev::port_type gyro_sensor = ev3dev::INPUT_3);

	EV3GripControl* GripControl();
	EV3DriveControl* DriveControl();
	EV3Sensing* Measure();
};

#endif