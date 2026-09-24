#ifndef TRIX_TRIANGLESOLVER_NORMALEXISTING_H
#define TRIX_TRIANGLESOLVER_NORMALEXISTING_H

#include <cmath>
#include <climits>
#include <cmath>
#include <array>
#include <string>

/*

C++ library to solve a triangle.
The input and output arrays are of the form [a, b, c, A, B, C] where uppercase letters are angle radians and lowercase in side lengths.
Values of zero or negative (<=0) or NaN are treated as unknowns to solve for.

If the returned value is [0, 0, 0, 0, 0, 0] = NIL then we have a problem and the triangle is unsolvable (i.e. a value is inconsistent with the rest of the triangle).
If the triangle is ambiguous then NIL is returned.

*/

namespace Trix {

typedef std::array<double, 6> Data;
constexpr Data NIL = {0, 0, 0, 0, 0, 0};
#define EPSILON 1e-12

inline bool isNil(Data d) {
	for (int i = 0; i < 6; i++) {
		if (d[i] != 0) return 0;
	}
	return 1;
}

inline double asinXX(double d) {
	double res = asin(d);
	if (res < 0) res += M_PI + M_PI;
	return res;
}

std::string disp(Trix::Data d, bool r2d = false) {
	constexpr double R2D = 180.0 / M_PI;
	std::string res = "Data[";
	for (int i = 0; i < 6; i++) {
		if (i) res += ", ";
		if (i >= 3 && r2d) res += std::to_string(d[i] * R2D);
		else res += std::to_string(d[i]);
	}
	return res + "]";
}

// [A, B, C] --> [B, C, A]
inline void rotate(Data& d, bool (&v)[6]) {
	for (int i = 0; i < 6; i += 3) {
		double td = d[i];
		d[i] = d[i + 1];
		d[i + 1] = d[i + 2];
		d[i + 2] = td;
		bool tb = v[i];
		v[i] = v[i + 1];
		v[i + 1] = v[i + 2];
		v[i + 2] = tb;
	}
}

inline bool isValid(double x) {
	return (x > EPSILON);
}

inline bool fequals(double a, double b) {
	return abs(a - b) < EPSILON;
}

inline bool checkContradiction(double computed, double given, bool earlyValid) {
	if (!earlyValid) return true;
	if (!isValid(given)) return true;
	return fequals(computed, given);
}

inline bool checkSides(double a, double b, double c) {
	return (a + b > c && b + c > a && c + a > b);
}

// c^2 = a^2 + b^2 - 2ab cos(C) --> cos(C) = -[c^2 - a^2 - b^2] / 2ab
inline double LoC_C(double a, double b, double c) {
	double ab = a * b;
	return acos((a * a + b * b - c * c) / (ab + ab));
}

inline double LoC_c(double a, double b, double C) {
	double ab2 = a * b;
	ab2 = ab2 + ab2;
	return sqrt(a * a + b * b - ab2 * cos(C));
}

Data solve(Data input) {
	bool v[6];
	for (int i = 0; i < 6; i++) v[i] = isValid(input[i]);

	// if no sides are provided then NIL is returned
	if (!v[0] && !v[1] && !v[2]) return NIL;

	// Case 1: all three sides are provided
	if (v[0] && v[1] && v[2]) {
		if (!checkSides(input[0], input[1], input[2])) return NIL;

		double A = LoC_C(input[1], input[2], input[0]);
		if (!checkContradiction(A, input[3], v[3])) return NIL;
		double B = LoC_C(input[2], input[0], input[1]);
		if (!checkContradiction(B, input[4], v[4])) return NIL;
		double C = LoC_C(input[0], input[1], input[2]);
		if (!checkContradiction(C, input[5], v[5])) return NIL;

		if (!fequals(A + B + C, M_PI)) return NIL;

		return {input[0], input[1], input[2], A, B, C};
	}

	bool someSide = v[0] || v[1] || v[2];

	int rotationCounter;
	bool solved = false;
	for (rotationCounter = 0; rotationCounter < 3; rotationCounter++) {
		// Case 2/3: two angles (A, B) and any side is provided
		if (v[3] && v[4] && someSide) {
			// compute the final angle
			input[5] = M_PI - input[3] - input[4];
			// check for zero angle
			if (!isValid(input[5])) return NIL;

			double R2 = 0;
			int theIndex = -1;
			// Law of sines: 2R = x / sin(X)
			for (int i = 0; i < 3; i++) {
				if (v[i]) {
					theIndex = i;
					R2 = input[i] / sin(input[i + 3]);
					break;
				}
			}
			if (theIndex < 0) return NIL;

			// x = 2R * sin(X)
			double a = (theIndex == 0) ? input[0] : R2 * sin(input[3]);
			if (!checkContradiction(a, input[0], v[0])) return NIL;
			double b = (theIndex == 1) ? input[1] : R2 * sin(input[4]);
			if (!checkContradiction(b, input[1], v[1])) return NIL;
			double c = (theIndex == 2) ? input[2] : R2 * sin(input[5]);
			if (!checkContradiction(c, input[2], v[2])) return NIL;

			input[0] = a;
			input[1] = b;
			input[2] = c;
			solved = true;

			break;
		}

		// Case 4: Two sides (a, b) and the angle between them (C)
		if (v[0] && v[1] && v[5]) {
			// solve for the other side
			double c = LoC_c(input[0], input[1], input[5]);
			if (!isValid(c)) return NIL;
			if (!checkContradiction(c, input[2], v[2])) return NIL;
			if (!checkSides(input[0], input[1], c)) return NIL;

			input[2] = c;

			// solve for the angles
			// a / sin(A) = 2R --> A = asin(a / 2R)
			double R2 = input[2] / sin(input[5]);
			if (!isValid(R2)) return NIL;
			double invR2 = 1.0 / R2;

			// the smaller angle is the angle facing the shorter side of the triangle (input[0], input[1]?)
			if (input[0] < input[1]) {
				double A = asin(input[0] * invR2);
				if (!checkContradiction(A, input[3], v[3])) return NIL;
				input[3] = A;
				double B = M_PI - input[3] - input[5];
				if (!checkContradiction(B, input[4], v[4])) return NIL;
				input[4] = B;
			} else {
				double B = asin(input[1] * invR2);
				if (!checkContradiction(B, input[4], v[4])) return NIL;
				input[4] = B;
				double A = M_PI - input[4] - input[5];
				if (!checkContradiction(A, input[3], v[3])) return NIL;
				input[3] = A;
			}
			solved = true;
			break;
		}

		rotate(input, v);
	}
	if (!solved) return NIL;
	if (rotationCounter == 0) return input;
	for (; rotationCounter < 3; rotationCounter++) rotate(input, v);
	return input;
}

}

#endif