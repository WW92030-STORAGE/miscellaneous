#include <bits/stdc++.h>
using namespace std;
#include "Fire.h"


int main() {
	DoomFire fire(256, 128, time(0));
	fire.SRC = 64;
	fire.RIGHT_MARGIN = 0;
	fire.defaultSources();

	fire.clear();
	for (int i = 0; i < 1024; i++) fire.update();
	cout << fire.buffer() << endl;

	return 0;
}
