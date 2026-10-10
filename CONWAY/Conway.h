#ifndef CONWAY_H
#define CONWAY_H

#include <set>
#include <array>
#include <string>

struct ConwayPoint {
	int64_t x;
	int64_t y;

	bool operator==(const ConwayPoint& other) const {
		return (x == other.x) && (y == other.y);
	}

	bool operator<(const ConwayPoint& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}

	ConwayPoint operator+(const ConwayPoint& other) {
		return ConwayPoint{x + other.x, y + other.y};
	}

	ConwayPoint min(const ConwayPoint& other) {
		return {std::min(x, other.x), std::min(y, other.y)};
	}

	ConwayPoint max(const ConwayPoint& other) {
		return {std::max(x, other.x), std::max(y, other.y)};
	}
};

// https://ianyepan.github.io/posts/cpp-custom-hash/
// https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
namespace std {
    template <>
    struct hash<ConwayPoint> {
        size_t operator()(const ConwayPoint& p) const {
            size_t h1 = hash<int>()(p.x);
            size_t h2 = hash<int>()(p.y);
            return h1 ^ (h2 + (h1<<6) + (h2>>2));
        }
    };
}

struct Conway {
	// use std::vector<bool> because it compresses
	std::unordered_set<ConwayPoint> points; 
	inline static const std::array<int, 8> dx = {1, 1, 0, -1, -1, -1, 0, 1};
	inline static const std::array<int, 8> dy = {0, 1, 1, 1, 0, -1, -1, -1};

	// Any point outside of these bounds will be ignored
	ConwayPoint BL;
	ConwayPoint TR;

	Conway(int64_t R = 1024) {
		BL = {-R, -R};
		TR = {R, R};
	}

	bool contains(ConwayPoint p) {
		return points.find(p) != points.end();
	}

	void remove(ConwayPoint p) {
		auto x = points.find(p);
		if (x != points.end()) points.erase(x);
	}

	void add(ConwayPoint p) {
		if (!inBounds(p)) return;
		points.insert(p);
	}

	void clear() {
		points.clear();
	}

	// If you set BL == TR the grid becomes infinite (aside from integer looping issues)
	bool inBounds(ConwayPoint p) {
		if (BL == TR) return true;
		if (p.x < BL.x || p.x > TR.x || p.y < BL.y || p.y > TR.y) return false;
		return true;
	}

	std::pair<ConwayPoint, ConwayPoint> boundingBox() {
		if (points.size() <= 0) return {ConwayPoint{0, 0}, ConwayPoint{0, 0}};
		ConwayPoint BL = *points.begin();
		ConwayPoint TR = BL;
		for (auto p : points) {
			BL = BL.min(p);
			TR = TR.max(p);
		}
		return {BL, TR};
	}

	void update() {
		std::unordered_set<ConwayPoint> toAdd;
		std::unordered_set<ConwayPoint> toDestroy;
		std::unordered_set<ConwayPoint> q;
		for (auto p : points) {
			q.insert(p);
			for (int i = 0; i < 8; i++) {
				ConwayPoint xp = {p.x + dx[i], p.y + dy[i]};
				if (inBounds(xp)) q.insert(xp);
			}
		}

		for (auto p : q) {
			int count = 0;
			for (int i = 0; i < 8; i++) {
				ConwayPoint xp = {p.x + dx[i], p.y + dy[i]};
				count += inBounds(xp) && contains(xp);
			}
			if (contains(p)) {
				if (count < 2) toDestroy.insert(p);
				if (count > 3) toDestroy.insert(p);
			} else if (count == 3) toAdd.insert(p); 
		}
		for (auto p : toDestroy) remove(p);
		for (auto p : toAdd) add(p);
	}

	std::string disp(ConwayPoint BL = {-16, -16}, ConwayPoint TR = {16, 16}, bool border = true, char empty = '.', char occupied = 'X') {
		int width = TR.x - BL.x + 1;
		int height = TR.y - BL.y + 1;
		std::string res = "";
		std::string top = "";
		if (border) {
			top.push_back('+');
			for (int i = 0; i < width; i++) top.push_back('-');
			top.push_back('+');
			res += top;
			res.push_back('\n');
		}
		for (int y = TR.y; y >= BL.y; y--) {
			std::string row = "";
			if (border) row.push_back('|');
			for (int x = BL.x; x <= TR.x; x++) {
				row.push_back(contains({x, y}) ? occupied : empty);
			}
			if (border) row.push_back('|');
			row.push_back('\n');
			res += (row);
		}
		if (border) res += (top);
		return res;
	}

	std::string dispBB(int margin = 0, ConwayPoint BL = {-64, -64}, ConwayPoint TR = {64, 64}, bool border = true, char empty = '.', char occupied = 'X') {
		auto bb = boundingBox();
		ConwayPoint BLx = bb.first + ConwayPoint{-margin, -margin};
		ConwayPoint TRx = bb.second + ConwayPoint{margin, margin};

		return disp(BL.max(BLx), TR.min(TRx), border, empty, occupied);
	}

	void translate(ConwayPoint a) {
		std::unordered_set<ConwayPoint> newPoints;
		for (auto p : points) newPoints.insert({p.x + a.x, p.y + a.y});
		std::swap(points, newPoints);
	}
	
	void rotate(int i = 1) {
		i &= 3;
		std::unordered_set<ConwayPoint> newPoints;
		for (auto p : points) {
			ConwayPoint q = {p.x, p.y};
			for (int a = 0; a < i; a++) q = {-q.y, q.x};
			newPoints.insert(q);
		}
		std::swap(points, newPoints);
	}

	void flipV() {
		std::unordered_set<ConwayPoint> newPoints;
		for (auto p : points) newPoints.insert({-p.x, p.y});
		std::swap(points, newPoints);
	}

	void flipH() {
		std::unordered_set<ConwayPoint> newPoints;
		for (auto p : points) newPoints.insert({p.x, -p.y});
		std::swap(points, newPoints);
	}
};

// Loaders

namespace ConwayPlaintext { // Everything in here loads from a strictly plaintext string representing a plaintext file: https://conwaylife.com/wiki/Plaintext

Conway loadString(std::string s, char empty = '.', char comm = '!') {
	Conway c;
	int prev = 0;
	int y = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '\n') {
			if (s[prev] == comm) {
				prev = i + 1;
				continue;
			}

			int xpos = 0;
			for (int x = prev; x < i; x++) {
				if (s[x] != empty) c.add({xpos, y});
				xpos++;
			}

			prev = i + 1;
			y--;
		}
	}

	if (s[prev] != comm) {
		int xpos = 0;
		for (int x = prev; x < s.size(); x++) {
			if (s[x] != empty) c.add({xpos, y});
			xpos++;
		}
	}
	return c;
}

Conway exportString(Conway& c) {

}

}

// Presets

namespace ConwayPresets {

void piHeptomino(Conway& conway) {
	conway.add({0, 1});
	conway.add({1, 1});
	conway.add({-1, 1});
	conway.add({1, 0});
	conway.add({-1, 0});
	conway.add({1, -1});
	conway.add({-1, -1});
}

void pentaDecathlon(Conway& conway) {
	for (int i = 0; i < 10; i++) conway.add({i, 0});
}

void gosperGun(Conway& conway) {
	const std::string input = "........................O.................................O.O.......................OO......OO............OO...........O...O....OO............OOOO........O.....O...OO..............OO........O...O.OO....O.O.....................O.....O.......O......................O...O................................OO......................";

	int ctr = 0;
	for (int y = 8; y >= 0; y--) {
		for (int x = 0; x < 36; x++) {
			if (input[ctr++] == 'O') conway.add({x, y});
		}
	}
}

};

#endif
