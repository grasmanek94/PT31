#ifndef HEADER_ISensing_hxx
#define HEADER_ISensing_hxx

#include "IComponentAvailability.hxx"

class ISensing : public virtual IComponentAvailability
{
public:
	virtual float GetFrontDistance() const = 0;
};

#endif
