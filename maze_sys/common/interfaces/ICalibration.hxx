#ifndef HEADER_ICalibration_hxx
#define HEADER_ICalibration_hxx

#include "IComponentAvailability.hxx"

class ICalibration : public virtual IComponentAvailability
{
public:
	virtual void Reload() = 0;
};

#endif
