#include <bits/stdc++.h>
using namespace std;

#include "Trix.h"

void runTests() {
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
	assert(Trix::fequals(out[2], sqrt(3)));
	assert(Trix::fequals(out[3], M_PI / 6));
	assert(Trix::fequals(out[4], M_PI / 2));

	Trix::Data d6 = {0, 1, sqrt(3), M_PI / 2, 0, 0};
	out = Trix::solve(d6);
	assert(Trix::fequals(out[0], 2));
	assert(Trix::fequals(out[5], M_PI / 3));
	assert(Trix::fequals(out[4], M_PI / 6));
}

// random problems procured by doing IXL's free trial (the assertion is that by inputting the parameters into the array, the output correctly answers the question)
void fieldtests() {
	double d2r = M_PI / 180;
	Trix::Data d1 = {20, 0, 0, 96 * d2r, 0, 47 * d2r};
	cout << Trix::disp(Trix::solve(d1), true) << endl;

	Trix::Data d2 = {15, 0, 0, 95 * d2r, 45 * d2r, 0};
	cout << Trix::disp(Trix::solve(d2), true) << endl;

	Trix::Data d3 = {0, 0, 10, 119 * d2r, 0, 41 * d2r};
	cout << Trix::disp(Trix::solve(d3), true) << endl;

	Trix::Data d4 = {17, 0, 0, 75 * d2r, 0, 57 * d2r};
	cout << Trix::disp(Trix::solve(d4), true) << endl;

	Trix::Data d5 = {17, 0, 10, 0, 15 * d2r, 0};
	cout << Trix::disp(Trix::solve(d5), true) << endl;

	Trix::Data d6 = {15, 0, 0, 0, 59 * d2r, 19 * d2r};
	cout << Trix::disp(Trix::solve(d6), true) << endl;
}

int main() {
	runTests();

	fieldtests();

	return 0;
}