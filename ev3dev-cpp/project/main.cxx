#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <atomic>

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
	static const int		_grip_amount = 2250;//90 degree turn
	ev3dev::medium_motor	_grip;
	ev3dev::touch_sensor	_touch_sensor;
	State					_state;

	bool _sensor_is_open() const
	{
		return _touch_sensor.value();
	}

public:
	GripControl(	ev3dev::port_type grip_motor, 
					ev3dev::port_type grip_sensor)

		:			_grip(grip_motor),
					_touch_sensor(grip_sensor)
	{
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

	~GripControl()
	{
		Reset();
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
		return _state;
	}

	bool Close()
	{
		if (_sensor_is_open() && 
			_state == StateOpen)
		{

			_state = StateClosing;

			_grip
				.set_position_sp(-_grip_amount)
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

	bool Open()
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

	bool Available() const
	{
		return 
			_grip.connected() && 
			_touch_sensor.connected();
	}
};

class DriveControl
{
public:
	enum State
	{
		StateStopped,
		StateMoving,
		StateHitSomething,
		StateTurning
	};

	enum Direction
	{
		DirectionLeft,
		DirectionRight
	};

private:
	ev3dev::large_motor     _left;
	ev3dev::large_motor     _right;
	ev3dev::touch_sensor	_touch_sensor;
	ev3dev::gyro_sensor		_gyro_sensor;
	State					_state;

	bool _sensor_hit_something() const
	{
		return _touch_sensor.value();
	}

public:
	DriveControl(	ev3dev::port_type left_motor, 
					ev3dev::port_type right_motor, 
					ev3dev::port_type obstruction_sensor,
					ev3dev::port_type gyro_sensor)

		:			_left(left_motor),
					_right(right_motor),
					_touch_sensor(obstruction_sensor),
					_gyro_sensor(gyro_sensor),
					_state(StateStopped)
	{
		//duplicate port usage check
		std::vector<ev3dev::port_type> ports(
			{ left_motor, right_motor, obstruction_sensor, gyro_sensor }
		);
		std::sort(ports.begin(), ports.end());
		if ((std::unique(ports.begin(), ports.end()) != ports.end()))
		{
			throw std::invalid_argument("Single port used for multiple devices");
		}

		Reset();
	}

	~DriveControl()
	{
		//Reset();
	}

	float GetRelativeDegrees()
	{
		return _gyro_sensor.float_value();
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

		_state = StateStopped;
	}

	void Move(int speed)
	{
		_state = StateMoving;

		_left
			.set_duty_cycle_sp(speed)
			.run_forever();

		_right
			.set_duty_cycle_sp(-speed)
			.run_forever();
	}

	void Turn(int speed, Direction direction, float bias, float degrees)
	{
		_state = StateTurning;

		float fspeed = (float)speed;

		if (degrees < 0.0)
		{
			degrees *= -1.0; 
			direction = (Direction)(direction ^ 1);
		}

		if (direction == DirectionLeft)
		{
			degrees /= 1.04;//4% error correction
		}
		else
		{
			degrees *= 1.02;//2% error correction
		}

		int dir_mult = direction == DirectionRight ? 1 : -1;

		float bias_l = direction == DirectionRight ? (1.0 - bias) : bias;
		float bias_r = 1.0 - bias_l;

		float left_speed = fspeed * bias_l;
		float right_speed = fspeed * bias_r;

		_left
			.set_duty_cycle_sp(dir_mult * (int)left_speed)
			.run_forever();

		_right
			.set_duty_cycle_sp(dir_mult * (int)right_speed)
			.run_forever();

		int gyro_start = _gyro_sensor.value();

		while (abs(_gyro_sensor.value() - gyro_start) < (int)degrees)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		_left.stop();
		_right.stop();

		_state = StateStopped;
	}

	bool Available() const
	{
		return 
			_left.connected() && 
			_right.connected() && 
			_touch_sensor.connected() &&
			_gyro_sensor.connected();
	}
};

class Sensing
{
private:
	ev3dev::ultrasonic_sensor _ultrasonic_sensor;
public:
	Sensing(	ev3dev::port_type distance_sensor)

		:		_ultrasonic_sensor(distance_sensor)
	{
	}

	float GetFrontDistance() const
	{
		return _ultrasonic_sensor.float_value();
	}

	bool Available() const
	{
		return _ultrasonic_sensor.connected();
	}
};

class Robot
{
private:
	GripControl _GripControl;
	DriveControl _DriveControl;
	Sensing _Measure;

public:
	Robot(	ev3dev::port_type _grip_motor = ev3dev::OUTPUT_B,
			ev3dev::port_type _grip_sensor = ev3dev::INPUT_1,
			ev3dev::port_type left_motor = ev3dev::OUTPUT_A,
			ev3dev::port_type right_motor = ev3dev::OUTPUT_D,
			ev3dev::port_type obstruction_sensor = ev3dev::INPUT_2,
			ev3dev::port_type distance_sensor = ev3dev::INPUT_4,
			ev3dev::port_type gyro_sensor = ev3dev::INPUT_3)

		:	_GripControl(_grip_motor, _grip_sensor),
			_DriveControl(left_motor, right_motor, obstruction_sensor, gyro_sensor),
			_Measure(distance_sensor)
	{
		//duplicate port usage check
		std::vector<ev3dev::port_type> ports(
			{ _grip_motor, _grip_sensor, left_motor, right_motor, obstruction_sensor, distance_sensor, gyro_sensor }
		);
		std::sort(ports.begin(), ports.end());
		if ((std::unique(ports.begin(), ports.end()) != ports.end()))
		{
			throw std::invalid_argument("Single port used for multiple devices");
		}
	}

	GripControl& Grip()
	{
		return _GripControl;
	}

	DriveControl& Drive()
	{
		return _DriveControl;
	}

	Sensing& Measure()
	{
		return _Measure;
	}
};

int main(int argc, char** argv)
{
	Robot robot;

	if (argc != 5)
	{
		return 0;
	}

	int speed = std::stoi(argv[1]);
	int direction = std::stoi(argv[2]);
	float bias = std::stof(argv[3]);
	float degrees = std::stof(argv[4]);

	std::cout << "S: " << speed << " D: " << direction << " B: " << bias << " *: " << degrees << std::endl;

	try
	{
		robot.Drive().Turn(speed, (DriveControl::Direction)direction, bias, degrees);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception occured: " << ex.what() << std::endl;
	}

	std::cout << "Exitting..." << std::endl;
	return 0;
}
