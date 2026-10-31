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
#include <iostream>

// Stronger WFC implementation (as opposed to the naive (area * patterns) from before)
// based on https://discourse.threejs.org/t/building-a-high-performance-wave-function-collapse-solver-for-three-js/81704

template <typename T>
	std::vector<std::vector<T>> sliceMatrix(std::vector<std::vector<T>>& v, int sr, int sc, int R, int C) {
		std::vector<std::vector<T>> res(R, std::vector<T>(C));
		for (int i = 0; i < R; i++) {
			int r = (i + sr) % v.size();
			for (int j = 0; j < C; j++) {
				int c = (j + sc) % v[r].size();
				res[i][j] = v[r][c];
			}
		}
		return res;
	}

std::string dispBitArray(std::vector<bool>& v) {
	std::string res = "";
	for (int i = 0; i < v.size(); i++) {
		if (v[i]) {
			if (res.size()) res.push_back(' ');
			res += std::to_string(i);
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
	std::vector<std::vector<T>> rotate(std::vector<std::vector<T>>& v) {
		int SR = v.size();
		int SC = v[0].size();
		std::vector<std::vector<T>> res(SC, std::vector<T>(SR));
		for (int r = 0; r < SR; r++) {
			for (int c = 0; c < SC; c++) res[c][r] = v[r][SC - 1-  c];
		}
		return res;
	}

template <typename T>
struct WFC {
	std::vector<std::vector<T>> spawn;
	int FR = 0;
	int FC = 0;

	std::vector<std::pair<std::pair<int, int>, int>> counts;

	std::mt19937 prng;

	void setseed(uint64_t sed) {
		prng = std::mt19937(sed);
	}

	int randint(int x) {
		std::uniform_int_distribution<int> distrib(0.0, x - 1);
		return distrib(prng);
	}

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

	void init(int fr = 3, int fc = 3, bool rotations = 0, bool loop = 0, bool verbose = 0) {
		int SR = spawn.size();
		int SC = spawn[0].size();

		FR = std::max(fr, 1);
		FC = std::max(fc, 1);

		if (SR < FR || SC < FC) return;

		std::map<std::pair<int, int>, int> cnts;
		std::map<std::vector<std::vector<T>>, std::pair<int, int>> principal;

		for (int i = 0; i < 4; i++) {

			for (int r = 0; r <= SR - (loop ? 1 : FR); r++) {
				for (int c = 0; c <= SC - (loop ? 1 : FC); c++) {
					std::vector<std::vector<T>> subgrid = sliceMatrix<T>(spawn, r, c, FR, FC);
					if (principal.find(subgrid) == principal.end()) principal[subgrid] = {r, c};
					cnts[principal[subgrid]]++;
				}
			}
			if (!rotations) break;
			spawn = rotate<T>(spawn);
		}

		counts.clear();
		for (auto p : cnts) counts.push_back(p);
		int P = counts.size();
		// Process neighbors. neighbors[i][j] is the set of possible neighbors in the (dx[j], dy[j]) direction of pattern i. 
		// Essentially, the set of neighbors taken by a region covering all but one row or column of pattern i, and arbitrary cells in the last column.
		neighbors = std::vector<std::vector<std::vector<int>>>(P, std::vector<std::vector<int>>(4, std::vector<int>()));

		generateMasks();

		for (int p = 0; p < P; p++) {
			auto pattern = sliceMatrix(spawn, counts[p].first.first, counts[p].first.second, FR, FC);
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
					auto location = counts[q].first;
					auto qattern = sliceMatrix(spawn, location.first, location.second, FR, FC);
					if (match(slicies, qattern, masks[d])) neighbors[p][d].push_back(q);
				}
			}
		}

		masks.clear();

		if (!verbose) return;
		for (int i = 0; i < P; i++) {
			auto pattern = sliceMatrix(spawn, counts[i].first.first, counts[i].first.second, FR, FC);
			std::cout << disp<T>(pattern) << endl;
			std::cout << i << ": \n";
			for (int d = 0; d < 4; d++) {
				std::cout << "> " << d << ": ";
				for (auto x : neighbors[i][d]) std::cout << x << " ";
				std::cout << endl;
			}
		}
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

	// Updated to use amortized arrays of the log-sum and the denominator (counts)
	bool singleAttempt(std::vector<std::vector<T>>& res, std::vector<std::vector<std::vector<bool>>>& possible, std::vector<std::vector<bool>>& generated, int& generatedCount, std::vector<std::vector<int>>& count_, std::vector<std::vector<double>>& countlog_, bool verbose = false) {
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
			// entropy = -( [1/count_] * countlog_ - log(count_))
			if (count_[r][c] <= 0) {
				if (verbose) std::cout << "ERROR(len)!!! " << r << " " << c << "\n";
				return false;
			}
			double entropy = -(countlog_[r][c] / count_[r][c] - log(count_[r][c]));
			// cout << r << " " << c << " " << entropy << "\n";
			if (entropy < minEntropy) {
				minEntropy = entropy;
				xr = r;
				xc = c;
			}
				if (entropy <= 0) {

				}
		}
		// end entropy pick
		if (xr < 0 || xc < 0) {
			if (verbose) std::cout << "ERROR(xr, xc)!!! " << xr << " " << xc << "\n";
			return false; // contradiction
		}
		// populate the chosen pattern index and counts
		for (int i = 0; i < P; i++) {
			if (possible[xr][xc][i]) {
				thePatterns.push_back(i);
				theCounts.push_back(counts[i].second);
			}
		}
		if (theCounts.size() <= 0) {
			if (verbose) std::cout << "ERROR (counts)!!! " << xr << " " << xc << "\n";
			return false; // contradiction
		}
		int index = randomIndex(theCounts);
		int chosenIndex = thePatterns[index];
		generated[xr][xc] = 1;
		generatedCount++;
		for (int fr = 0; fr < FR; fr++) {
			auto pattern = sliceMatrix(spawn, counts[chosenIndex].first.first, counts[chosenIndex].first.second, FR, FC);
			for (int fc = 0; fc < FC; fc++) res[fr + xr][fc + xc] = pattern[fr][fc];
		}

		for (int i = 0; i < P; i++) possible[xr][xc][i] = (i == chosenIndex);

		// propagate changes
		std::queue<std::pair<int, int>> q;
		q.push({xr, xc});
		// cout << "BEGIN " << chosenIndex << "\n";
		std::vector<bool> computedPossible(P, 0);
		while (q.size()) {
			auto p = q.front();
			int r = p.first;
			int c = p.second;
			q.pop();
			for (int d = 0; d < 4; d++) {
				int pr = r + dx[d];
				int pc = c + dy[d];
				if (pr < 0 || pc < 0 || pr >= generated.size() || pc >= generated[pr].size()) continue;

				for (int i = 0; i < P; i++) computedPossible[i] = 0;

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
					if (possible[pr][pc][i] != original) {
						reduced = true;
						// also diminish the count and countlog values
						int cc = counts[i].second;
						count_[pr][pc] -= cc;
						countlog_[pr][pc] -= cc * log(cc);
					}
				}
				if (reduced) {
					q.push({pr, pc});
					bool hasP = false;
					for (auto x : possible[pr][pc]) hasP |= x;
					if (!hasP) {
						if (verbose) std::cout << "ERROR(propagation)" << pr << " " << pc << "\n";
						return false;
					}
				}
			}
		} // end queue loop

		return true;
	}

	std::vector<std::vector<T>> generate(int R, int C, bool verbose = false) {
		int generatedCount = 0;
		int P = counts.size();
		std::vector<std::vector<T>> res(R, std::vector<T>(C));
		std::vector<std::vector<std::vector<bool>>> possible(R - FR + 1, std::vector<std::vector<bool>>(C - FC + 1, std::vector<bool>(P, 1)));
		std::vector<std::vector<bool>> generated(R - FR + 1, std::vector<bool>(C - FC + 1, 0));

		/*
		
		entropy = sum[probability * log(probability)]. probability = count / denom so
		entropy = sum[(count/denom) * log(count/denom)] = [1/denom]sum(count * [log(count) - log(denom)])
		= [1/denom]sum(count * log(count)) - [1/denom]sum(count * log(denom)). denom = sum(count) thus 
		entropy = [1/denom]sum(count * log(count)) - log(denom)

		therefore we only need to keep a total count_ = sum(count) and a countlog_ = sum(count * log(count)) 
		since entropy = [1/count_] * countlog_ - log(count_)

		well actually entropy is -1 times that but yeah basically

		std::vector<std::pair<std::vector<std::vector<T>>, int>> counts;
		
		*/
		double commonCount = 0;
		double commonCountLog = 0;

		for (int i = 0; i < P; i++) {
			commonCount += counts[i].second;
			commonCountLog += counts[i].second * log(counts[i].second);
		}

		std::vector<std::vector<int>> count_(R - FR + 1, std::vector<int>(C - FC + 1, commonCount));
		std::vector<std::vector<double>> countlog_(R - FR + 1, std::vector<double>(C - FC + 1, commonCountLog));

		int totalAtts = (R - FR + 1) * (C - FC + 1);

		for (int attempt = 0; attempt < totalAtts; attempt++) {
			if (!singleAttempt(res, possible, generated, generatedCount, count_, countlog_, verbose)) break;
			if (verbose && (attempt % 16) == 0) std::cout << "ATTEMPT " << (attempt + 1) << "/" << totalAtts << " DONE\n";
		}

		if (verbose) std::cout << generatedCount << "/" << (R - FR + 1) * (C - FC + 1) << " CELLS COMPLETED\n";

		return res;
	}

	std::string dispCounts() {
		std::string res = "";
		for (auto p : counts) {
			auto pattern = sliceMatrix(spawn, p.first.first, p.first.second, FR, FC);
			res += disp(pattern) + "\n> " + std::to_string(p.second) + "\n\n";
		}
		return res;
	}
};

namespace WFCUtils {

template <typename T>
std::string buffer(std::vector<std::vector<T>>& v) {
	int R = v.size();
	int C = v[0].size();
	std::string res = "[" + std::to_string(R) + "," + std::to_string(C) + "]";
	for (auto r : v) {
		res.push_back('\n');
		for (auto c : r) res += std::to_string(c) + ",";
	}
	return res;
}

}

#endif
