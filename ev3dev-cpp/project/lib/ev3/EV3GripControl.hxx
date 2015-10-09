#ifndef HEADER_EV3GripControl_hxx
#define HEADER_EV3GripControl_hxx

#include <interfaces/IGripControl.hxx>
#include <ev3dev/ev3dev.hxx>

#include "EV3Calibration.hxx"

class EV3GripControl : public IGripControl
{
private:
	EV3Calibration*			_Calibration;
	ev3dev::medium_motor	_grip;
	ev3dev::touch_sensor	_touch_sensor;
	State					_state;

	inline bool _sensor_is_open() const
	{
		return _touch_sensor.value();
	}
public:
	EV3GripControl(	EV3Calibration* calibration,
					ev3dev::port_type grip_motor = ev3dev::OUTPUT_B,
					ev3dev::port_type grip_sensor = ev3dev::INPUT_1);
	~EV3GripControl();

	void Reset();
	State GetState() const;
	bool Close();
	bool Open();
	bool Available() const;
};

#endif