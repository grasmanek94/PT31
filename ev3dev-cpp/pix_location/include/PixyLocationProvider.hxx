#ifndef PixyLocationProvider_hpp
#define PixyLocationProvider_hpp

#include <stdio.h>
#include <stdexcept>

#include "pixy.h"

#include "LocationProvider.hxx"

#define BLOCK_BUFFER_SIZE 25

class PixyLocationProvider : public LocationProvider {
	public:
		PixyLocationProvider();
		~PixyLocationProvider();
		Location getLocation(int id);
};

#endif
