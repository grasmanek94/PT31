#ifndef HEADER_ILocation_hxx
#define HEADER_ILocation_hxx

#include "IComponentAvailability.hxx"
#include <atomic>

struct sPosition
{
	float x;
	float y;
	float z;
	float a;
};

struct vsPosition
{
	std::atomic<float> x;
	std::atomic<float> y;
	std::atomic<float> z;
	std::atomic<float> a;
};

class ILocation : public virtual IComponentAvailability
{
public:
	//return x y z angle
	virtual sPosition GetLocation() = 0;
};

#endif
