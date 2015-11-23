#include <thread>
#include <chrono>

#include <ev3dev/ev3dev.hxx>

#include "EV3Calibration.hxx"
#include "EV3GripControl.hxx"

EV3GripControl::EV3GripControl(	
		EV3Calibration* calibration,
		ev3dev::port_type grip_motor,
		ev3dev::port_type grip_sensor)

	:	_Calibration(calibration),
		_grip(grip_motor),
		_touch_sensor(grip_sensor)
{
	if (calibration == nullptr)
	{
		throw std::invalid_argument("calibration == nullptr");
	}

	if (grip_motor == grip_sensor)
	{
		throw std::invalid_argument("grip_motor == grip_sensor");
	}

	Reset();

	if (Available())
	{
		//Only reliable way to determine state is by the touch sensor, so use it!
		if (_sensor_is_open())
		{
			_state = StateOpen;
		}
		else
		{
			_state = StateClosed;
			Open();
		}
	}
	else
	{
		_state = StateUnknown; //Please connect the motor and sensor to the correct ports
	}
}

EV3GripControl::~EV3GripControl()
{
	Reset();
}

void EV3GripControl::Reset()
{
	if (_grip.connected())
	{
		_grip.reset();
	}
}

EV3GripControl::State EV3GripControl::GetState() const
{
	return _state;
}

bool EV3GripControl::Close()
{
	if (_sensor_is_open() &&
		_state == StateOpen)
	{

		_state = StateClosing;

		_grip
			.set_position_sp(-_Calibration->GetGripCloseAmount())
			.set_duty_cycle_sp(100)
			.run_to_rel_pos();

		while (_grip.state().count("running"))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		_state = StateClosed;

		return true;
	}

	return false;
}

bool EV3GripControl::Open()
{
	if (!_sensor_is_open() &&
		_state == StateClosed)
	{

		_state = StateOpening;

		_grip
			.set_duty_cycle_sp(100)
			.run_forever();

		while (!_sensor_is_open())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		_grip.stop();

		_state = StateOpen;
	}

	return false;
}

bool EV3GripControl::Available() const
{
	return
		_grip.connected() &&
		_touch_sensor.connected();
}
