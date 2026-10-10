#include <bits/stdc++.h>
using namespace std;
#include "Conway.h"


int main() {
	Conway conway(0);
	ConwayPresets::acorn(conway);

	for (int i = 0; i < 2000; i++) conway.update();

	cout << conway.dispBB(2) << endl;

	return 0;
}
