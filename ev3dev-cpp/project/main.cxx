#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#define KEY_RELEASE 0
#define KEY_PRESS   1
#define KEY_REPEAT  2
#endif

#include <lib/ev3dev/ev3dev.hxx>

/*
	Run this class in another thread because this blocks the thread (all motor control functions do)
*/
class GripControl
{
public:
	enum State
	{
		StateOpen,
		StateClosed,
		StateOpening,
		StateClosing,
		StateUnknown
	};

private:
	static const int		_grip_amount = 2050;//90 degree turn
	ev3dev::medium_motor	_grip;
	ev3dev::touch_sensor	_touch_sensor;
	State					state;

	bool _sensor_is_open() const
	{
		return _touch_sensor.value();
	}

public:
	GripControl()
		:	_grip(ev3dev::OUTPUT_B),
		_touch_sensor(ev3dev::INPUT_1)
	{
		Reset();
		
		if (Available())
		{
			//Only reliable way to determine state is by the touch sensor, so use it!
			if (_sensor_is_open())
			{
				state = StateOpen;
			}
			else
			{
				state = StateClosed;
				Open();
			}
		}
		else
		{
			state = StateUnknown;
		}
	}

	void Reset()
	{
		if (_grip.connected())
		{
			_grip.reset();
		}
	}

	State GetState() const
	{
		return state;
	}

	bool Close()
	{
		if (_sensor_is_open() && 
			state == StateOpen)
		{

			state = StateClosing;

			_grip
				.set_position_sp(-_grip_amount)
				.set_duty_cycle_sp(100)
				.run_to_rel_pos();

			while (_grip.state().count("running"))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			state = StateClosed;

			return true;
		}

		return false;
	}

	bool Open()
	{
		if (!_sensor_is_open() &&
			state == StateClosed)
		{

			state = StateOpening;

			_grip
				.set_duty_cycle_sp(100)
				.run_forever();

			while (!_sensor_is_open())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			_grip.stop();

			state = StateOpen;
		}

		return false;
	}

	bool Available() const
	{
		return _grip.connected() && _touch_sensor.connected();
	}
};

class MotorControl
{
public:
	MotorControl()
		:	_left(ev3dev::OUTPUT_A), 
			_right(ev3dev::OUTPUT_D)
	{
		Reset();
	}

	~MotorControl()
	{
		//Reset();
	}

	void Reset()
	{
		if (_left.connected())
		{
			_left.reset();
		}

		if (_right.connected())
		{
			_right.reset();
		}
	}

	void Move(int speed)
	{
		_left.set_duty_cycle_sp(speed);
		_right.set_duty_cycle_sp(-speed);
		_left.run_forever();
		_right.run_forever();
	}

	bool Available() const
	{
		return _left.connected() && _right.connected();
	}

private:
	ev3dev::large_motor     _left;
	ev3dev::large_motor     _right;


};

int main()
{
	GripControl control;

	try
	{
		if (control.Available())
		{
			std::cout << "Running motors" << std::endl;
			control.Open();
			control.Close();
		}
		else
		{
			std::cout << "Control unavailable" << std::endl;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception occured: " << ex.what() << std::endl;
	}

	std::cout << "Exitting..." << std::endl;
	return 0;
}
