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

EV3DriveControl::~EV3DriveControl()
{
	Reset();
}

float EV3DriveControl::GetRelativeDegrees()
{
	return _gyro_sensor.float_value();
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

	while (_left.state().count("running") || _right.state().count("running"))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	_state = StateStopped;
}

void EV3DriveControl::Turn(int speed, Direction direction, float bias, float degrees)
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
		degrees *= 0.96;//-4% error correction
	}
	else
	{
		degrees *= 1.02;//+2% error correction
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

	while (abs(GetRelativeDegrees() - gyro_start) < degrees)
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