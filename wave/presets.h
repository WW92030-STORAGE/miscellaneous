#include <vector>

namespace WFCPresets {

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

}