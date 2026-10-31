#include <bits/stdc++.h>
using namespace std;
#include "Poissondisk.h"


int main() {
	auto x = PoissonDisk::PoissonDisk2D();
	for (auto i : x) cout << "(" << i.first << ", " << i.second << ")\n";

	return 0;
}
