#include <bits/stdc++.h>
using namespace std;
#include "Wave.h"
#include "presets.h"


int main() {
	auto spawnMap = WFCPresets::square();

	for (auto r : spawnMap) {
		for (auto c : r) cout << c << " ";
		cout << endl;
	}


	WFC<bool> wfc;
	wfc.setseed(time(0));
	wfc.spawn = spawnMap;
	wfc.init(3, 3, false, true, false);

	auto x = wfc.generate(16, 16);

	for (auto r : x) {
		for (auto c : r) cout << (c ? '#' : '.');
		cout << endl;
	}

	return 0;
}
