#ifndef PixyLocationProvider_hpp
#define PixyLocationProvider_hpp

#include <stdio.h>

#include "pixy.h"

#include "LocationProvider.hxx"

class PixyLocationProvider : public LocationProvider {
	public:
		PixyLocationProvider();
		~PixyLocationProvider();
		Location getLocation();
};

#endif
