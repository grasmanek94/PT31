#include "EV3Sensing.hxx"

EV3Sensing::EV3Sensing(ev3dev::port_type distance_sensor)
	: _ultrasonic_sensor(distance_sensor)
{
}

float EV3Sensing::GetFrontDistance() const
{
	return _ultrasonic_sensor.float_value();
}

bool EV3Sensing::Available() const
{
	return _ultrasonic_sensor.connected();
}