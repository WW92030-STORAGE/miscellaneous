#ifndef WAVE_H_NORMALEXISTING
#define WAVE_H_NORMALEXISTING

#include <vector>
#include <map>
#include <random>
#include <cstdint>
#include <cfloat>
#include <queue>
#include <utility>
#include <algorithm>
#include "Counter.h"
	
template <typename T>
	std::vector<std::vector<T>> sliceMatrix(std::vector<std::vector<T>>& v, int sr, int sc, int R, int C) {
		std::vector<std::vector<T>> res(R, std::vector<T>(C));
		for (int i = 0; i < R; i++) {
			int r = i + sr;
			if (r >= v.size()) break;
			for (int j = 0; j < C; j++) {
				int c = j + sc;
				if (c >= v[r].size()) break;
				res[i][j] = v[r][c];
			}
		}
		return res;
	}

template <typename T>
	std::string disp(std::vector<std::vector<T>> v) {
		std::string res = "";
		for (int r = 0; r < v.size(); r++) {
			if (r) res += "\n";
			res.push_back('[');
			for (int c = 0; c < v[r].size(); c++) {
				if (c) res += ", ";
				res += std::to_string(v[r][c]);
			}
			res.push_back(']');
		}
		return res;
	}

template <typename T>
struct WFC {
	std::vector<std::vector<T>> spawn;
	int FR = 0;
	int FC = 0;

	std::vector<std::pair<std::vector<std::vector<T>>, int>> counts;

	std::mt19937 prng;

	const int dx[4] = {1, 0, -1, 0};
	const int dy[4] = {0, 1, 0, -1};

	std::vector<std::vector<std::vector<bool>>> masks;
	std::vector<std::vector<std::vector<int>>> neighbors;

	// look, most people store the allowed patterns that directly neighbor (i.e. shifted by 1 in each direction) so i'm going to do that.
	void generateMasks() {
		masks = std::vector<std::vector<std::vector<bool>>>(4);

		for (int d = 0; d < 4; d++) {
			masks[d] = std::vector<std::vector<bool>>(FR, std::vector<bool>(FC, 1));
			// begin mask reset
			for (int fr = 0; fr < FR; fr++) {
				masks[d][fr][0] = (d != 3);
				masks[d][fr][FC - 1] = (d != 1);
			}
			for (int fc = 0; fc < FC; fc++) {
				masks[d][0][fc] = (d != 2);
				masks[d][FR - 1][fc] = (d != 0);
			}
			masks[d][0][0] = (d == 0 || d == 1);
			masks[d][0][FC - 1] = (d == 3 || d == 0);
			masks[d][FR - 1][FC - 1] = (d == 2 || d == 3);
			masks[d][FR - 1][0] = (d == 1 || d == 2);
			// end mask reset
		}
	}

	void init(int fr = 3, int fc = 3) {
		int SR = spawn.size();
		int SC = spawn[0].size();

		FR = std::max(fr, 1);
		FC = std::max(fc, 1);

		if (SR < FR || SC < FC) return;

		Counter<std::vector<std::vector<T>>> cnts;

		for (int r = 0; r <= SR - FR; r++) {
			for (int c = 0; c <= SC - FC; c++) {
				std::vector<std::vector<T>> subgrid = sliceMatrix<T>(spawn, r, c, FR, FC);
				cnts.push(subgrid);
			}
		}

		counts.clear();
		for (auto p : cnts.counter) counts.push_back(p);
		int P = counts.size();
		// Process neighbors. neighbors[i][j] is the set of possible neighbors in the (dx[j], dy[j]) direction of pattern i. 
		// Essentially, the set of neighbors taken by a region covering all but one row or column of pattern i, and arbitrary cells in the last column.
		neighbors = std::vector<std::vector<std::vector<int>>>(P, std::vector<std::vector<int>>(4, std::vector<int>()));

		generateMasks();

		for (int p = 0; p < P; p++) {
			auto pattern = counts[p].first;
			std::vector<std::vector<T>> slicies(FR, std::vector<T>(FC));
			for (int d = 0; d < 4; d++) {
				
				switch (d) {
					case 0:
					for (int fr = 0; fr < FR - 1; fr++) {
						for (int fc = 0; fc < FC; fc++) slicies[fr][fc] = pattern[fr + 1][fc];
					} break;
					case 1:
					for (int fr = 0; fr < FR; fr++) {
						for (int fc = 0; fc < FC - 1; fc++) slicies[fr][fc] = pattern[fr][fc + 1];
					} break;
					case 2:
					for (int fr = 1; fr < FR; fr++) {
						for (int fc = 0; fc < FC; fc++) slicies[fr][fc] = pattern[fr - 1][fc];
					} break;
					case 3:
					for (int fr = 0; fr < FR; fr++) {
						for (int fc = 1; fc < FC; fc++) slicies[fr][fc] = pattern[fr][fc - 1];
					} break;
				}

				for (int q = 0; q < P; q++) {
					auto qattern = counts[q].first;
					if (match(slicies, qattern, masks[d])) neighbors[p][d].push_back(q);
				}
			}
		}
	}

	void setseed(uint64_t sed) {
		prng = std::mt19937(sed);
	}

	int randint(int x) {
		std::uniform_int_distribution<int> distrib(0.0, x - 1);
		return distrib(prng);
	}

	

	int randomIndex(std::vector<int>& v) {
		int sum = 0;
		for (auto i : v) sum += i;
		int choice = randint(sum);
		sum = 0;
		for (int i = 0; i < v.size(); i++) {
			sum += v[i];
			if (choice < sum) return i;
		}
		return 0;
	}

	bool match(std::vector<std::vector<T>>& pattern, std::vector<std::vector<T>>& region, std::vector<std::vector<bool>>& wassup) {
		if (pattern.size() != region.size() || region.size() != wassup.size() || pattern.size() != wassup.size()) return 0;
		if (pattern[0].size() != region[0].size() || region[0].size() != wassup[0].size() || pattern[0].size() != wassup[0].size()) return 0;

		for (int r = 0; r < pattern.size(); r++) {
			for (int c = 0; c < pattern[r].size(); c++) {
				if (wassup[r][c] && pattern[r][c] != region[r][c]) return false;
			}
		}
		return true;
 	}

	bool singleAttempt(std::vector<std::vector<T>>& res, std::vector<std::vector<std::vector<bool>>>& possible, std::vector<std::vector<bool>>& generated) {
		int R = res.size();
		int C = res[0].size();
		int P = counts.size();

		// search through all blocks
		int xr = -1;
		int xc = -1;
		double minEntropy = DBL_MAX;
		std::vector<int> thePatterns;
		std::vector<int> theCounts;

		std::vector<std::pair<int, int>> randomOrder;
		for (int r = 0; r <= R - FR; r++) {
			for (int c = 0; c <= C - FC; c++) randomOrder.push_back({r, c});
		}

		std::shuffle(randomOrder.begin(), randomOrder.end(), prng);

		for (int ii = 0; ii < randomOrder.size(); ii++) {
			int r = randomOrder[ii].first;
			int c = randomOrder[ii].second;
				if (generated[r][c]) continue;
				int len = 0; 
				for (int i = 0; i < P; i++) len += possible[r][c][i];
				if (len <= 0) {
					// std::cout << "ERROR!!! " << r << " " << c << "\n";
					return false;
				}
				if (len <= 1) continue; // generated
				// compute entropy
				int denom = 0;
				for (int i = 0; i < P; i++) {
					if (possible[r][c][i]) denom += counts[i].second;
				}
				if (denom <= 0) return false; // contradiction
				double trueDenom = 1.0 / denom;
				double entropy = 0;
				for (int i = 0; i < P; i++) {
					if (possible[r][c][i]) {
						double prob = counts[i].second * trueDenom;
						entropy -= prob * log(prob);
					}
				}
				// cout << r << " " << c << " " << entropy << "\n";
				if (entropy < minEntropy) {
					minEntropy = entropy;
					xr = r;
					xc = c;
				}
		}
		// end entropy pick
		if (xr < 0 || xc < 0) return false; // contradiction
		// populate the chosen pattern index and counts
		for (int i = 0; i < P; i++) {
			if (possible[xr][xc][i]) {
				thePatterns.push_back(i);
				theCounts.push_back(counts[i].second);
			}
		}
		if (theCounts.size() <= 0) return false; // contradiction
		int index = randomIndex(theCounts);
		int chosenIndex = thePatterns[index];
		generated[xr][xc] = 1;
		for (int fr = 0; fr < FR; fr++) {
			for (int fc = 0; fc < FC; fc++) res[fr + xr][fc + xc] = counts[chosenIndex].first[fr][fc];
		}
		// cout << xr << " " << xc << "\n";

		for (int i = 0; i < P; i++) possible[xr][xc][i] = (i == chosenIndex);

		// propagate changes
		std::queue<std::pair<int, int>> q;
		q.push({xr, xc});
		while (q.size()) {
			auto p = q.front();
			int r = p.first;
			int c = p.second;
			q.pop();
			for (int d = 0; d < 4; d++) {
				int pr = r + dx[d];
				int pc = c + dy[d];
				if (pr < 0 || pc < 0 || pr >= generated.size() || pc >= generated[pr].size()) continue;

				std::vector<bool> computedPossible(P, 0);

				bool reduced = false;
				// iterate through all currently known values at the starting point
				for (int i = 0; i < P; i++) {
					if (possible[r][c][i]) { // the current index is possible.
						for (auto n : neighbors[i][d]) computedPossible[n] = 1;
					}
				}
				// compute possibles at the neighboring point
				for (int i = 0; i < P; i++) {
					bool original = possible[pr][pc][i];
					possible[pr][pc][i] = possible[pr][pc][i] & computedPossible[i];
					if (possible[pr][pc][i] != original) reduced = true;
				}
				if (reduced) q.push({pr, pc});
			}
		}

		return true;
	}

	std::vector<std::vector<T>> generate(int R, int C) {
		int P = counts.size();
		std::vector<std::vector<T>> res(R, std::vector<T>(C));
		std::vector<std::vector<std::vector<bool>>> possible(R - FR + 1, std::vector<std::vector<bool>>(C - FC + 1, std::vector<bool>(P, 1)));
		std::vector<std::vector<bool>> generated(R - FR + 1, std::vector<bool>(C - FC + 1, 0));

		for (int attempt = 0; attempt < (R - FR + 1) * (C - FC + 1); attempt++) {
			singleAttempt(res, possible, generated);
		}

		return res;
	}

	std::string dispCounts() {
		std::string res = "";
		for (auto p : counts) res += disp(p.first) + "\n> " + std::to_string(p.second) + "\n\n";
		return res;
	}
};

#endif
