#include <interfaces/IRobot.hxx>
#include <ev3dev/ev3dev.hxx>

#include "EV3Calibration.hxx"
#include "EV3DriveControl.hxx"
#include "EV3GripControl.hxx"

#include "EV3Robot.hxx"

EV3Robot::EV3Robot(
	ev3dev::port_type grip_motor,
	ev3dev::port_type grip_sensor,
	ev3dev::port_type left_motor,
	ev3dev::port_type right_motor,
	ev3dev::port_type obstruction_sensor,
	ev3dev::port_type distance_sensor,
	ev3dev::port_type gyro_sensor)

	:	_GripControl(&_Calibration, grip_motor, grip_sensor),
		_DriveControl(&_Calibration,left_motor, right_motor, obstruction_sensor, gyro_sensor, distance_sensor)
{
	//duplicate port usage check
	std::vector<ev3dev::port_type> ports(
		{ grip_motor, grip_sensor, left_motor, right_motor, obstruction_sensor, distance_sensor, gyro_sensor }
	);
	std::sort(ports.begin(), ports.end());
	if ((std::unique(ports.begin(), ports.end()) != ports.end()))
	{
		throw std::invalid_argument("Single port used for multiple devices");
	}
}

EV3GripControl* EV3Robot::GripControl()
{
	return &_GripControl;
}

EV3DriveControl* EV3Robot::DriveControl()
{
	return &_DriveControl;
}

EV3Calibration* EV3Robot::Calibration()
{
	return &_Calibration;
}
