#ifndef HEADER_ILocation_hxx
#define HEADER_ILocation_hxx

#include "IComponentAvailability.hxx"

struct xyza
{
	float x;
	float y;
	float z;
	float a;
};

class ILocation : public virtual IComponentAvailability
{
public:
	//return x y z angle
	virtual xyza GetLocation() = 0;
};

#endif
