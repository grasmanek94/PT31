#ifndef HEADER_ILocation_hxx
#define HEADER_ILocation_hxx

struct xyza
{
	float x;
	float y;
	float z;
	float a;
};

class ILocation
{
public:
	//return x y z angle
	virtual xyza GetLocation() = 0;
	virtual bool Available() const = 0;
};

#endif
