#ifndef HEADER_IGripControl_hxx
#define HEADER_IGripControl_hxx

#include "IComponentAvailability.hxx"

class IGripControl : public virtual IComponentAvailability
{
public:
	enum State
	{
		StateUnknown,
		StateClosed,
		StateClosing,
		StateOpen,
		StateOpening,
	};
	
	//Returns true if closing, false if already closed or closing
	virtual bool Close() = 0;

	//Returns true if opening, false if already open or opening
	virtual bool Open() = 0;

	virtual State GetState() const = 0;
};

#endif
