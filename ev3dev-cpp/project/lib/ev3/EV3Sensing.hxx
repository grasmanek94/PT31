#ifndef HEADER_EV3Sensing_hxx
#define HEADER_EV3Sensing_hxx

#include <interfaces/ISensing.hxx>
#include <ev3dev/ev3dev.hxx>

class EV3Sensing : public ISensing
{
private:
	ev3dev::ultrasonic_sensor _ultrasonic_sensor;
public:
	EV3Sensing(ev3dev::port_type distance_sensor = ev3dev::INPUT_4);
	float GetFrontDistance() const;
	bool Available() const;
};

#endif