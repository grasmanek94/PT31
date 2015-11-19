#ifndef HEADER_ISensing_hxx
#define HEADER_ISensing_hxx

class ISensing
{
public:
	virtual float GetFrontDistance() const = 0;
	virtual bool Available() const = 0;
};

#endif
