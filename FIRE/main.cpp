#include <bits/stdc++.h>
using namespace std;
#include "DoomFire.h"

int main() {
	DoomFire fire(256, 128, time(0));
	fire.SRC = 64;
	fire.RIGHT_MARGIN = 0;
	fire.defaultSources();

	fire.clear();
	DoomFireUtils::animate(fire, 256, "video");

	return 0;
}
