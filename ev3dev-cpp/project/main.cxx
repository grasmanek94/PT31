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
		//Reset();
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

		if(_grip.connected())
		{
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

	void Close()
	{
		_grip
			.set_position_sp(10000)
			.set_duty_cycle_sp(33)
			.run_to_rel_pos();
	}

	void Open()
	{
		_grip
			.set_position_sp(-10000)
			.set_duty_cycle_sp(33)
			.run_to_rel_pos();
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
			control.Close();
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			control.Open();
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
