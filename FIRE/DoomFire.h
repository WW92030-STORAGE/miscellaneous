#ifndef DOOMFIRE_NORMALEXISTING_H__
#define DOOMFIRE_NORMALEXISTING_H__

#include <vector>
#include <cstdint>
#include <random>
#include <string>
#include <fstream>

namespace DoomFireConsts {
	std::uniform_real_distribution<double> rand01(0.0, 1.0);

	constexpr char SYMBOLS[] = {
			' ', '.', '_', '-', 
			':', '~', '=', '|', 
			'/', '+', '*', 'o', 
			'O', '8', '%', '#'
		};
	constexpr int N_SYM = 16;

	typedef int16_t PARTICLE;
}

struct DoomFire {
	uint64_t SEED = 0;
	DoomFireConsts::PARTICLE SRC = 36;
	int R = 16;
	int C = 16;
	double LEFT_MARGIN = 1.0 / 3.0;
	double RIGHT_MARGIN = 1.0 / 3.0;
	double DECAY = 2.0 / 3.0;
	DoomFireConsts::PARTICLE** particles;
	bool** sources;
	std::mt19937 prng;
	

	// https://fabiensanglard.net/doom_fire_psx/

	void init(uint64_t seed) {
		SEED = seed;
		prng = std::mt19937(seed);
		particles = new DoomFireConsts::PARTICLE*[R];
		sources = new bool*[R];
		for (int i = 0; i < R; i++) {
			particles[i] = new DoomFireConsts::PARTICLE[C];
			sources[i] = new bool[C];
		}
		defaultSources();
		clear();
	}

	void defaultSources() {
		for (int r = 0; r < R; r++) {
			for (int c = 1; c < C; c++) sources[r][c] = 0;
			sources[r][0] = 1;
		}
	}

	void clearSources() {
		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) sources[r][c] = 0;
		}
	}

	DoomFire() {
		init(0);
	}

	DoomFire(uint64_t seed) {
		init(seed);
	}

	DoomFire(int r, int c, uint64_t seed) {
		R = r;
		C = c;
		init(seed);
	}

	DoomFire(int r, int c) {
		R = r;
		C = c;
		init(0);
	}


	DoomFire(const DoomFire& other) {
		R = other.R;
		C = other.C;
		LEFT_MARGIN = other.LEFT_MARGIN;
		RIGHT_MARGIN = other.RIGHT_MARGIN;
		prng = std::mt19937(other.prng);
		SRC = other.SRC;
		SEED = other.SEED;
		DECAY = other.DECAY;
		init(SEED);

		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) particles[r][c] = other.particles[r][c];
		}
	}

	~DoomFire() {
		for (int r = 0; r < R; r++) delete[] particles[r];
		delete[] particles;
	}

	void clear() {
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) particles[i][j] = sources[i][j] * SRC;
		}
	}

	void update() {
		for (int r = 0; r < R; r++) {
			for (int c = C - 2; c >= 0; c--) spread(r, c);
		}
	}

	double random() {
		return DoomFireConsts::rand01(prng);
	}

	void spread(int r, int c) {
		if (c >= C - 1) return;
		DoomFireConsts::PARTICLE value = particles[r][c];
		value -= (random() < DECAY);

		double offsetValue = random();
		int offset = offsetValue < LEFT_MARGIN ? -1 : (offsetValue >= (1 - RIGHT_MARGIN) ? 1 : 0);
		int rp = r + offset;

		// post processing
		if (rp < 0 || rp >= R) return;
		if (value < 0) value = 0;
		if (sources[rp][c + 1]) return;
		particles[rp][c + 1] = value;
	}

	std::string to_string(uint8_t B = 16, bool border = true) {
		std::string res = "DoomFire[" + std::to_string(R) + ", " + std::to_string(C) + "]";
		res.push_back('\n');
		return res + disp(B, border);
	}

	std::string disp(int B = 16, bool border = true) {
		if (B > 16) B = 16;
		double denom = 1.0 / (SRC + 1);

		std::string res = "";
		if (border) {
			res.push_back('+');
			for (int i = 0; i < R; i++) res.push_back('-');
			res += "+\n";
		}
		for (int c = C - 1; c >= 0; c--) {
			if (c != C - 1) res.push_back('\n');
			if (border) res.push_back('|');
			for (int r = 0; r < R; r++) {
				double xx = (double)(particles[r][c]) * denom;
				int bandno = (int)(DoomFireConsts::N_SYM * xx);
				res.push_back(DoomFireConsts::SYMBOLS[bandno]);
			}
			if (border) res.push_back('|');
		}

		if (border) {
			res += "\n+";
			for (int i = 0; i < R; i++) res.push_back('-');
			res.push_back('+');
		}
		return res;
	}

	std::string buffer() {
		int B = SRC;

		std::string res = "[" + std::to_string(B) + ", " + std::to_string(R) + ", " + std::to_string(C) + "]";
		for (int c = C - 1; c >= 0; c--) {
			res.push_back('\n');
			for (int r = 0; r < R; r++) {
				res += std::to_string(int(particles[r][c])) + ",";
			}
		}
		return res;
	}
};

namespace DoomFireUtils {
	void animate(DoomFire world, int framecount, std::string OUT_DIR, bool verbose = false) {
    	for (int i = 0; i < framecount; i++) {
			std::string FILE_OUT = OUT_DIR + "/FRAME_" + std::to_string(i);
    	    world.update();
    		std::ofstream output(FILE_OUT);
    		output << world.buffer();
    		output.close();
    	}

    	std::ofstream len(OUT_DIR + "/LEN");
    	len << framecount;
    	len.close();
	}
}

#endif
