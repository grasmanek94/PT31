#ifndef LocationProvider_hpp
#define LocationProvider_hpp

struct Location {
	int x;
	int y;
};

class LocationProvider {
	public:
		virtual Location getLocation(int id) = 0;
};

#endif
