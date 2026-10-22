#include <bits/stdc++.h>
using namespace std;
#include "Wave.h"


int main() {
	std::vector<std::vector<bool>> spawnMap({
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 0, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
	});

	WFC<bool> wfc;
	wfc.spawn = spawnMap;
	wfc.init(2, 2);

	auto x = wfc.generate(64, 64);

	for (auto r : x) {
		for (auto c : r) cout << (c ? '#' : '.');
		cout << endl;
	}

	return 0;
}
