#ifndef HEADER_IGripControl_hxx
#define HEADER_IGripControl_hxx

class IGripControl
{
public:
	enum State
	{
		StateUnknown,
		StateError,
		StateSuccess,
		StateNotConnected,

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
	//Does this robot have (a functioning) GripControl?
	virtual bool Available() const = 0;
};

#endif
