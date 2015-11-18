#ifndef UWBLocationProvider_hpp
#define UWBLocationProvider_hpp

#include <string>
#include <vector>

#include "Uwbips.h"
#include "LocationProvider.hxx"

class UWBLocationProvider : public LocationProvider {
    private:
        UWBIPS* uwb;
	public:
		UWBLocationProvider(const std::string& devname, vec3d anchor1, vec3d anchor2);
		~UWBLocationProvider();
		Location getLocation(int id);
};

#endif
