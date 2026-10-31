#ifndef WFCPRESETS_H_NORMALEXISTING
#define WFCPRESETS_H_NORMALEXISTING

#include <vector>

namespace WFCPresets {

std::vector<std::vector<int16_t>> boolmat2int16(std::vector<std::vector<bool>> v) {
  std::vector<std::vector<int16_t>> res;
  for (int i = 0; i < v.size(); i++) {
    res.push_back(std::vector<int16_t>());
    for (int j = 0; j < v[i].size(); j++) res[i].push_back(v[i][j]);
  }
  return res;
}

// Most of these presets are pulled from/based on either the official WFC repo

std::vector<std::vector<bool>> square() {
	std::vector<std::vector<bool>> spawnMap(9, std::vector<bool>(9, 0));
	for (int i = 2; i < 7; i++) {
		for (int j = 2; j < 7; j++) spawnMap[i][j] = 1;
	}
	for (int i = 3 ; i < 6; i++) {
		for (int j = 3; j < 6; j++) spawnMap[i][j] = 0;
	}
	return spawnMap;
}

// This one's from Wikipedia.
std::vector<std::vector<bool>> sun() {
	std::vector<std::vector<bool>> spawnMap({
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 1, 1, 1, 0, 0},
		{0, 1, 1, 0, 0, 0, 1, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 0, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 1, 0, 0, 0, 1, 1, 0},
		{0, 0, 1, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
	});
	return spawnMap;
}

std::vector<std::vector<bool>> infinity() {
	std::vector<std::vector<bool>> spawnMap({
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	});
	return spawnMap;
}

std::vector<std::vector<bool>> diamond() {
	std::vector<std::vector<bool>> spawnMap({
		{0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 1, 0, 1, 0, 0},
		{0, 0, 1, 0, 0, 0, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 1},
		{0, 0, 1, 0, 0, 0, 1, 0},
		{0, 0, 0, 1, 0, 1, 0, 0},
	});
	return spawnMap;
}

// use a 2x2 spawn area
std::vector<std::vector<bool>> tinyassgrid() {
	std::vector<std::vector<bool>> spawnMap({
		{0, 0, 0, 0},
		{0, 1, 1, 1},
		{0, 1, 0, 1},
		{0, 1, 1, 1},
	});
	return spawnMap;
}

// use a 2x2 spawn area
std::vector<std::vector<int16_t>> tinyasstricolor() {
	std::vector<std::vector<int16_t>> spawnMap({
		{0, 0, 0, 0},
		{0, 1, 1, 1},
		{0, 1, 2, 1},
		{0, 1, 1, 1},
	});
	return spawnMap;
}

}

#endif