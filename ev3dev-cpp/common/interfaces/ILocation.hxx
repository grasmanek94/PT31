#ifndef HEADER_ILocation_hxx
#define HEADER_ILocation_hxx

#include <glm/glm.hpp>

class ILocation
{
public:
	//return x y z angle
	virtual glm::vec4 GetLocation() = 0;
};

#endif