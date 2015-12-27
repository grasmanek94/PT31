#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#include <ev3dev/ev3dev.hxx>

#include "EV3DriveControl.hxx"

EV3DriveControl::EV3DriveControl(
		EV3Calibration* calibration,
		ev3dev::port_type left_motor,
		ev3dev::port_type right_motor,
		ev3dev::port_type obstruction_sensor,
		ev3dev::port_type gyro_sensor)

	:	_Calibration(calibration),
		_left(left_motor),
		_right(right_motor),
		_touch_sensor(obstruction_sensor),
		_gyro_sensor(gyro_sensor),
		_state(StateStopped),
		_stop_requested(false)
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

	_gyro_sensor.set_mode("GYRO-ANG");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	_gyro_sensor.set_mode("GYRO-G&A");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	Reset();
}

EV3DriveControl::~EV3DriveControl()
{
	Reset();
}

float EV3DriveControl::GetRelativeDegrees()
{
	return _gyro_sensor.float_value(0);
}

float EV3DriveControl::GetRotationalSpeed()
{
	return _gyro_sensor.float_value(1);
}

void EV3DriveControl::Reset()
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

void EV3DriveControl::Move(int speed, float centimeters)
{
	_state = StateMoving;

	if (speed < 40)
	{
		speed = 40;
	}
	else if (speed > 100)
	{
		speed = 100;
	}

	static const float units_per_cm = 500.0 / 17.0;
	int units = (int)(centimeters * units_per_cm);

	_left
		.set_duty_cycle_sp(speed)
		.set_position_sp(units)
		.run_to_rel_pos();

	_right
		.set_duty_cycle_sp(speed)
		.set_position_sp(-units)
		.run_to_rel_pos();
	
	int begin_angle = GetRelativeDegrees();

	int left_start_pos = _left.position();
	int right_start_pos = _right.position();

	int left_speed = speed;
	int right_speed = speed;

	int last_case = 0;
	int current_case = 0;
	int diff = 0;

	while 
		(	
			!_stop_requested && 
			(_left.state().count("running") && _right.state().count("running"))
		)
	{

		int current_angle = (int)GetRelativeDegrees();
		diff = begin_angle - current_angle;

		if(diff == 0)
		{
			left_speed = speed;
			right_speed = speed;

			current_case = 0;
		}
		else if (diff > 0)//left relative to start
		{
			//so we need to go more right
			left_speed = speed + speed / 7;
			right_speed = speed - speed / 7;

			current_case = 1;
		}
		else if (diff < 0)//right relative to start
		{
			//so we need to go more left
			left_speed = speed - speed / 7;
			right_speed = speed + speed / 7;

			current_case = 2;
		}

		if (current_case != last_case)
		{
			last_case = current_case;

			_right
				.set_duty_cycle_sp(right_speed)
				.set_position_sp((right_start_pos - _right.position()) - units)
				.run_to_rel_pos();

			_left
				.set_duty_cycle_sp(left_speed)
				.set_position_sp(units - (_left.position() - left_start_pos))
				.run_to_rel_pos();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	_left.stop();
	_right.stop();

	/*if (diff > 0)//left relative to start
	{
		//so we need to go more right
		Turn(speed / 2 + speed / 3, DirectionRight, 1.00, (float)diff);
	}
	else if (diff < 0)//right relative to start
	{
		//so we need to go more left
		Turn(speed / 2 + speed / 3, DirectionLeft, 1.00, (float)diff);
	}*/

	_state = StateStopped;
}

void EV3DriveControl::Turn(int speed, Direction direction, float bias, float degrees)
{
	_state = StateMoving;

	float fspeed = (float)speed;

	if (degrees < 0.0)
	{
		degrees *= -1.0;
		direction = (Direction)(direction ^ 1);
	}

	if (direction == DirectionLeft)
	{
		degrees *= _Calibration->GetLeftTurnCorrection();//-4% error correction
	}
	else
	{
		degrees *= _Calibration->GetRightTurnCorrection();//+2% error correction
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

	float gyro_start = GetRelativeDegrees();

	while 
		(
			!_stop_requested && 
			(abs(GetRelativeDegrees() - gyro_start) < degrees && (_left.state().count("running") || _right.state().count("running")))
		)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	_left.stop();
	_right.stop();

	_state = StateStopped;
}

bool EV3DriveControl::Available() const
{
	return
		_left.connected() &&
		_right.connected() &&
		_touch_sensor.connected() &&
		_gyro_sensor.connected();
}

EV3DriveControl::State EV3DriveControl::GetState() const
{
	return _state;
}

void EV3DriveControl::Stop()
{
	if (!_state != StateStopped)
	{
		_stop_requested = true;
		while (_state != StateStopped)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));//usleep(5000);
		}
		_stop_requested = false;
	}

	_left.stop();
	_right.stop();

	_state = StateStopped;
}
