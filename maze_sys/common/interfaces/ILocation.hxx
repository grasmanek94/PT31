#ifndef HEADER_ILocation_hxx
#define HEADER_ILocation_hxx

class ILocation
{
public:
	//return x y z angle
	virtual void GetLocation() = 0;
	virtual bool Available() const = 0;
};

#endif
