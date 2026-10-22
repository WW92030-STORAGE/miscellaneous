#include <bits/stdc++.h>
using namespace std;
#include "Wave.h"
#include "presets.h"


int main() {
	auto spawnMap = WFCPresets::infinity();
	WFC<bool> wfc;
	wfc.setseed(time(0));
	wfc.spawn = spawnMap;
	wfc.init(3, 3, false, true, false);

	int R = 64;
	int C = 64;

	auto x = wfc.generate(R, C, false);

	cout << WFCUtils::buffer(x);

	return 0;

	for (auto r : x) {
		for (auto c : r) cout << (c ? '#' : '.');
		cout << endl;
	}

	return 0;
}
