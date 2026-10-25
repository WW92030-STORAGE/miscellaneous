#ifndef BOIDS_H_NORMALEXISTING
#define BOIDS_H_NORMALEXISTING

#include <vector>
#include <string>
#include <climits>
#include <fstream>

namespace Boids {

typedef float BoidsNum;

BoidsNum rand01() {
	constexpr BoidsNum denom = 1.0 / (BoidsNum)(RAND_MAX);
	return (BoidsNum)(rand()) * denom;
}

bool fzero(BoidsNum b) {
	constexpr BoidsNum eps = 1e-12;
	return (b > -eps && b < eps);
}

struct Vec2 {
	BoidsNum x = 0;
	BoidsNum y = 0;

	void add(const Vec2& o) {
		x += o.x;
		y += o.y;
	}

	void sub(const Vec2& o) {
		x -= o.x;
		y -= o.y;
	}

	void mul(BoidsNum b) {
		x *= b;
		y *= b;
	}

	Vec2 operator+(const Vec2& o) {
		return {x + o.x, y + o.y};
	}

	Vec2 operator-(const Vec2& o) {
		return {x - o.x, y - o.y};
	}

	Vec2 operator*(float b) {
		return {x * b, y * b};
	}

	BoidsNum normsquared() {
		return x * x + y * y;
	}

	BoidsNum length() { return sqrt(normsquared()); }

	Vec2 normalized() {
		BoidsNum len = sqrt(normsquared());
		if (fzero(len)) return {x, y};
		BoidsNum inv = 1.0 / len;
		return {x * inv, y * inv};
	}

	Vec2 rect2polar() {
		return {length(), atan2(y, x)};
	}

	Vec2 polar2rect() {
		return {x * cos(y), x * sin(y)};
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	std::string buffer() {
		return std::to_string(x) + ", " + std::to_string(y);
	}
};

struct Boid {
	Vec2 pos, vel;
	std::string to_string() {
		return "Boid[" + pos.to_string() + ", " + vel.to_string() + "]";
	}
};

// Rectangular Boids simulation 
struct Boids {
	// constants with meters in the numerator (m, m/s)
	Vec2 BL = {-1, -1};
	Vec2 TR = {1, 1};
	BoidsNum protR = 0.05;
	BoidsNum visR = 0.3;
	BoidsNum margin = 0.1;
	BoidsNum minSpeed = 0.005;
	BoidsNum maxSpeed = 0.03;
	BoidsNum turn = 0.02;

	// dimensionless constants
	
	BoidsNum match = 0.05;
	BoidsNum avoid = 0.05;
	BoidsNum center = 0.0005;

	std::vector<Boid> boids;

	void add(Vec2 v) {
		boids.push_back(Boid{v});
	}

	// Based on https://people.ece.cornell.edu/land/courses/ece4760/labs/s2021/Boids/Boids.html
	void update() {
		BoidsNum protRsq = protR * protR;
		BoidsNum visRsq = visR * visR;

		int N = boids.size();

		// for each boid
		for (int i = 0; i < N; i++) {
			Vec2 pos = boids[i].pos;

			Vec2 v_avg{0, 0};
			Vec2 p_avg{0, 0};
			Vec2 close_disp{0, 0};
			int neighbors = 0;
			for (int j = 0; j < N; j++) {
				Vec2 disp = pos - boids[j].pos;
				BoidsNum rsq = disp.normsquared();

				// 1. spread
				if (rsq < protRsq) close_disp.add(disp);

				// 2. steer and align
				if (rsq < visRsq) {
					v_avg.add(boids[j].vel);
					neighbors++;

					// 3. center
					p_avg.add(boids[j].pos);
				}
			}

			// 1
			boids[i].vel.add(close_disp * avoid);

			// 2
			if (neighbors) {
				BoidsNum denom = 1.0f / neighbors;
				v_avg.mul(denom);
				boids[i].vel.add((v_avg - boids[i].vel) * match);

				// 3
				p_avg.mul(denom);
				boids[i].vel.add((p_avg - pos) * center);
			}

			// 4. avoid the margins
			if (pos.x < BL.x + margin) boids[i].vel.x += turn;
			if (pos.x > TR.x - margin) boids[i].vel.x -= turn;
			if (pos.y < BL.y + margin) boids[i].vel.y += turn;
			if (pos.y > TR.y - margin) boids[i].vel.y -= turn;

			// 5. regulate speed
			double speed = boids[i].vel.length();
			if (speed <= 0) continue;
			if (speed > maxSpeed) boids[i].vel.mul(maxSpeed / speed);
			if (speed < minSpeed) boids[i].vel.mul(minSpeed / speed);

			boids[i].pos.add(boids[i].vel);
		}
	}

	std::string to_string() {
		std::string res = "Boids[" + BL.to_string() + ", " + TR.to_string() + ", " + std::to_string(boids.size()) + "]";
		for (auto boid : boids) res += "\n" + boid.to_string();
		return res;
	}

	std::string buffer() {
		std::string res = "[" + BL.buffer() + ", " + TR.buffer() + ", " + std::to_string(boids.size()) + "]";
		for (auto boid : boids) res += "\n" + boid.pos.buffer() + ",";
		return res;
	}
};

	void animate(Boids& boids, int framecount, std::string OUT_DIR, bool verbose = false) {
    	for (int i = 0; i < framecount; i++) {
			std::string FILE_OUT = OUT_DIR + "/FRAME_" + std::to_string(i);
    	    boids.update();
    		std::ofstream output(FILE_OUT);
    		output << boids.buffer();
    		output.close();
    	}

    	std::ofstream len(OUT_DIR + "/LEN");
    	len << framecount;
    	len.close();
	}

}

#endif
