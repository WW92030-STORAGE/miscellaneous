#include <bits/stdc++.h>
using namespace std;

#include "Trix.h"

void runTests() {
	cout << Trix::asinXX(-0.5) << endl;
	Trix::Data d0 = {3, 4, 5, 0, 0, 0};

	Trix::Data out = Trix::solve(d0);
	assert(Trix::fequals(out[5], M_PI / 2));

	Trix::Data d1 = {1, 1, 1, 0, 0, 0};
	out = Trix::solve(d1);
	assert(Trix::fequals(out[3], M_PI / 3));
	assert(Trix::fequals(out[4], M_PI / 3));
	assert(Trix::fequals(out[5], M_PI / 3));

	Trix::Data d2 = {0, 0, 2, M_PI / 3, M_PI / 6, 0};
	out = Trix::solve(d2);
	assert(Trix::fequals(out[0], 2 * sqrt(0.75)));
	assert(Trix::fequals(out[1], 1));
	assert(Trix::fequals(out[5], M_PI / 2));

	Trix::Data d3 = {0, 1, 0, 0, M_PI / 6, M_PI / 2};
	out = Trix::solve(d3);
	assert(Trix::fequals(out[0], sqrt(3)));
	assert(Trix::fequals(out[1], 1));
	assert(Trix::fequals(out[5], M_PI / 2));

	Trix::Data d4 = {3, 4, 0, M_PI / 3, 0, M_PI / 2};
	out = Trix::solve(d4);
	assert(Trix::isNil(out));

	Trix::Data d5 = {1, 2, 0, 0, 0, M_PI / 3};
	out = Trix::solve(d5);
	cout << Trix::disp(out) << endl;
	assert(Trix::fequals(out[2], sqrt(3)));
	assert(Trix::fequals(out[3], M_PI / 6));
	assert(Trix::fequals(out[4], M_PI / 2));

	Trix::Data d6 = {0, 1, sqrt(3), M_PI / 2, 0, 0};
	out = Trix::solve(d6);
	cout << Trix::disp(out) << endl;
	assert(Trix::fequals(out[0], 2));
	assert(Trix::fequals(out[5], M_PI / 3));
	assert(Trix::fequals(out[4], M_PI / 6));
}

int main() {
	runTests();

	return 0;
}