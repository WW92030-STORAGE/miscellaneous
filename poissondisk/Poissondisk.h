#ifndef POISSONDISK_H_NORMALEXISTING
#define POISSONDISK_H_NORMALEXISTING

#include <utility>
#include <vector>
#include <map>
#include <climits>
#include <cfloat>

// https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

namespace PoissonDisk {

typedef float PDNumeric;
typedef std::pair<PDNumeric, PDNumeric> PDPoint2D;
typedef std::pair<int, int> PDHash2D;

// common

PDNumeric rand01() {
	constexpr PDNumeric denom = 1.0 / (PDNumeric)(RAND_MAX);
	return (PDNumeric)(rand()) * denom;
}

PDNumeric randab(PDNumeric a, PDNumeric b) {
	return a + (b - a) * rand01();
}

// 2D

PDNumeric normsquared2D(PDPoint2D p) {
	return p.first * p.first + p.second * p.second;
}

PDNumeric length2D(PDPoint2D p) {
	return sqrt(normsquared2D(p));
}

PDNumeric dist2D(PDPoint2D a, PDPoint2D b) {
	PDPoint2D c = {a.first - b.first, a.second - b.second};
	return length2D(c);
}

PDHash2D computeHash2D(PDPoint2D p, PDNumeric inv) {
	return {floor(p.first * inv), floor(p.second * inv)};
}

void push2D(std::map<PDHash2D, std::vector<int>>& grid, PDPoint2D p, int index, PDNumeric inv) {
	PDHash2D hash = computeHash2D(p, inv);
	if (grid.find(hash) == grid.end()) grid[hash] = std::vector<int>();
	grid[hash].push_back(index);
}

std::vector<PDPoint2D> PoissonDisk2D(PDNumeric r = 0.05, PDPoint2D BL = {0, 0}, PDPoint2D TR = {1, 1}, PDNumeric k = 30) {
	std::map<PDHash2D, std::vector<int>> grid;
	std::vector<int> active(1, 0);
	std::vector<PDPoint2D> res(1);
	if (r <= 0) return res;
	if (k <= 0) k = 1;

	PDNumeric gridSize = r * sqrt(0.5);
	PDNumeric invSize = 1.0 / r;
	
	// step 1. choose the initial sample
	PDPoint2D p0 = {randab(BL.first, TR.first), randab(BL.second, TR.second)};
	res[0] = p0;
	push2D(grid, p0, 0, invSize);

	// step 2. (do this as long as the active list is nonempty): 
	// 1. choose a random index i from it
	// 2. generate up to k random points around it, such that each point is within [r, 2r] distance from the original point p = res[i]
	// 3. for each point p, check if it is nearby other samples. teh background grid should be useful here.
	// 4. if a sample is found then append it as done in step 1 and add this index to the active list.
	// 5. otherwise remove from the active list. the active list is effectively a set of objects so you don't have to worry about the order.

	while (active.size()) {
		int slot = rand01() * active.size();
		int index = active[slot];
		PDPoint2D p = res[index];
		bool ultimateSatisfaction = false;
		for (int iter = 0; iter < k; iter++) {
			// generate a random point in the annulus
			// https://stackoverflow.com/questions/13064912/generate-a-uniformly-random-point-within-an-annulus-ring
			// sqrt(rand01()*([2R]^2-R^2)+R^2) = sqrt(rand01() * 3R^2 + R^2) = R * sqrt(rand01() * 3 + 1)
			PDNumeric theta = rand01() * M_PI * 2;
			PDNumeric rad = r * sqrt(rand01() * 3 + 1);
			PDPoint2D sample = {rad * cos(theta) + p.first, rad * sin(theta) + p.second};
			if (sample.first < BL.first || sample.first > TR.first || sample.second < BL.second || sample.second > TR. second) continue;

			PDHash2D hash = computeHash2D(sample, invSize);
			bool satisfied = true;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					PDHash2D other = {hash.first + dx, hash.second + dy};
					if (grid.find(other) == grid.end()) continue;
					if (grid[other].size() <= 0) continue;
					for (auto test : grid[other]) {
						if (dist2D(sample, res[test]) < r) {
							satisfied = false;
							break;
						}
					}
					if (!satisfied) break;
				}
				if (!satisfied) break;
			}
			if (satisfied) {
				ultimateSatisfaction = true;
				int theIndex = res.size();
				res.push_back(sample);
				active.push_back(theIndex);
				push2D(grid, sample, theIndex, invSize);
				break;
			}
		}
		if (!ultimateSatisfaction) {
			std::swap(active[slot], active[active.size() - 1]);
			active.pop_back();
		}
	}
	return res;
}



}

#endif
