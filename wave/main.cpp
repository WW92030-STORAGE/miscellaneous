#include <bits/stdc++.h>
using namespace std;
#include "Wave.h"


int main() {
	std::vector<std::vector<bool>> spawnMap(9, std::vector<bool>(9, 0));
	for (int i = 2; i < 7; i++) {
		for (int j = 2; j < 7; j++) spawnMap[i][j] = 1;
	}
	for (int i = 3 ; i < 6; i++) {
		for (int j = 3; j < 6; j++) spawnMap[i][j] = 0;
	}


	WFC<bool> wfc;
	wfc.setseed(time(0));
	wfc.spawn = spawnMap;
	wfc.init(3, 3);

	auto x = wfc.generate(16, 16);

	for (auto r : x) {
		for (auto c : r) cout << (c ? '#' : '.');
		cout << endl;
	}

	return 0;
}
