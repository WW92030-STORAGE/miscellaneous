#include <bits/stdc++.h>
using namespace std;
#include "Wave.h"
#include "WFCPresets.h"

void doBool() {
	auto spawnMap = WFCPresets::sun();
	WFC<bool> wfc;
	wfc.setseed(0);
	wfc.spawn = spawnMap;
	wfc.init(3, 3, false, true, false);

	int R = 32;
	int C = 32;

	auto x = wfc.generate(R, C, true);

	cout << WFCUtils::buffer(x);

	return;
}

void doInt16() {
	auto spawnMap = WFCPresets::tinyasstricolor();
	WFC<int16_t> wfc;
	wfc.setseed(time(0));
	wfc.spawn = spawnMap;
	wfc.init(3, 3, false, true, false);

	int R = 64;
	int C = 64;

	auto x = wfc.generate(R, C, false);

	cout << WFCUtils::buffer(x);
}

int main() {
	doBool();
	return 0;
}
