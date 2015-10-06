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

class MotorControl
{
public:
	MotorControl()
		: _left(ev3dev::OUTPUT_A), 
		_right(ev3dev::OUTPUT_D), 
		_grip(ev3dev::OUTPUT_B)
	{
		/*if (Available())
		{
			_left.reset();
			_right.reset();
			_grip.reset();
		}*/
	}

	~MotorControl()
	{
		if (Available())
		{
			_left.reset();
			_right.reset();
			_grip.reset();
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
		return _left.connected() && _right.connected() && _grip.connected();
	}

private:
	ev3dev::large_motor     _left;
	ev3dev::large_motor     _right;
	ev3dev::medium_motor    _grip;

};

int main()
{
	MotorControl control;

	try
	{
		if (control.Available())
		{
			std::cout << "Running motors" << std::endl;
			control.Move(33);
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

	return 0;
}
