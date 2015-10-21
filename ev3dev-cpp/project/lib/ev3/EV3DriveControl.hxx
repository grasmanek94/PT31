#ifndef HEADER_EV3DriveControl_hxx
#define HEADER_EV3DriveControl_hxx

#include <interfaces/IDriveControl.hxx>
#include <interfaces/ILocation.hxx>
#include <ev3dev/ev3dev.hxx>

#include "EV3Calibration.hxx"

class EV3DriveControl : public IDriveControl
{
private:
	EV3Calibration*			_Calibration;
	ev3dev::large_motor     _left;
	ev3dev::large_motor     _right;
	ev3dev::touch_sensor	_touch_sensor;
	ev3dev::gyro_sensor		_gyro_sensor;
	State					_state;

	inline bool _sensor_hit_something() const
	{
		return _touch_sensor.value();
	}

public:
	EV3DriveControl(EV3Calibration* calibration, 
					ev3dev::port_type left_motor = ev3dev::OUTPUT_A,
					ev3dev::port_type right_motor = ev3dev::OUTPUT_D,
					ev3dev::port_type obstruction_sensor = ev3dev::INPUT_2,
					ev3dev::port_type gyro_sensor = ev3dev::INPUT_3);

	~EV3DriveControl();
	//fwd | right + left -
	float GetRelativeDegrees();
	float GetRotationalSpeed();
	void Reset();
	void Move(int speed, float centimeters);
	void Turn(int speed, Direction direction, float bias, float degrees);
	State GetState() const;
	bool Available() const;
	void Stop();
};

#endif