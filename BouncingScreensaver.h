#ifndef DVD_H_NORMALEXISTING
#define DVD_H_NORMALEXISTING

#include <vector>
#include <cstdint>

// Something to model a bouncing image e.g. a bouncing DVD logo.

struct BouncingScreensaver {
	int BW = 64;
	int BH = 32;
	int LW = 9;
	int LH = 7;

	int x = 0;
	int y = 0;
	int8_t dx = 1;
	int8_t dy = 1;

	BouncingScreensaver(int bw = 64, int bh = 32, int lw = 9, int lh = 7) {
		BW = bw;
		BH = bh;
		LW = lw;
		LH = lh;
	}

	bool step() {
		bool ret = 0;
		if (dx == 1 && x >= BW - LW) {
			ret = 1;
			dx = -1;
		}
		else if (dx == -1 && x <= 0) {
			ret = 1;
			dx = 1;
		}
		if (dy == 1 && y >= BH - LH) {
			ret = 1;
			dy = -1;
		}
		else if (dy == -1 && y <= 0) {
			ret = 1;
			dy = 1;
		}
		x += dx;
		y += dy;
		return ret;
	}
};

#endif
