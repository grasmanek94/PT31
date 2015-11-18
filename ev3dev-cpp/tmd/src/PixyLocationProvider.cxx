#include "PixyLocationProvider.hxx"

PixyLocationProvider::PixyLocationProvider() {
	int status = pixy_init();
	
	if (status != 0) {
		throw std::runtime_error("Error initializing pixy");
	}
}

PixyLocationProvider::~PixyLocationProvider() {
	pixy_close();
}

Location PixyLocationProvider::getLocation(int id) {
	Location loc;
	loc.x = -1;
	loc.y = -1;
	
	if (!pixy_blocks_are_new()) {
		// nothing to report
		return loc;
	}
	
	struct Block blocks[BLOCK_BUFFER_SIZE];
	int new_blocks = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);
	
	if (new_blocks < 0) {
		// error or no blocks?
		return loc;
	}
	
	for (int i = 0; i < new_blocks; i++) {
		if (blocks[i].signature == id) {
			loc.x = blocks[i].x;
			loc.y = blocks[i].y;
			return loc;
		}
	}
	
	// all other cases
	return loc;
};
