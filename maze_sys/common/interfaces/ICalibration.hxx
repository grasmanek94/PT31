#ifndef HEADER_ICalibration_hxx
#define HEADER_ICalibration_hxx

#include <string>
#include <map>

class ICalibration
{
public:
	virtual void Reload() = 0;
	virtual bool Available() const = 0;
};

#endif