#include <bits/stdc++.h>
using namespace std;
#include "Conway.h"


int main() {
	Conway conway(64);
	ConwayPresets::gosperGun(conway);
	for (int i = 0; i < 10000; i++) conway.update();

	cout << conway.dispBB(2) << endl;

	return 0;
}
