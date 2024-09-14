#include <vector>
#include <cmath>

// CODE TO RETRIEVE THE CENTERS OF THE TWO CIRCLES THROUGH TWO POINTS ps[0] AND ps[1] AND WITH RADIUS r.

double sq(double x) {
	return x * x;
}
std::vector<double> circ(std::vector<std::vector<int>> ps, int r) {
	std::vector<double> res(4, 0); // centers
	std::vector<double> mid = {0.5 * (ps[0][0] + ps[1][0]), 0.5 * (ps[0][1] + ps[1][1])}; // midpoint of AB which is M
	std::vector<double> ortho = {-1.0 * (ps[0][1] - ps[1][1]), (double)(ps[0][0]) - ps[1][0]};

	double norm = sqrt(sq(ortho[0]) + sq(ortho[1]));
	for (int i = 0; i < 2; i++) ortho[i] /= norm; // unit std::vector
	double schlope = ortho[1] / ortho[0]; // slope of theline connecting the two centers
	// y - y0 = slope(x - x0) so y = slope * x + (y0 - slope * x0)
	double b = mid[1] - schlope * mid[0]; // y intercept of that line

	double dp2msq = (sq(ps[0][0] - mid[0]) + sq(ps[0][1] - mid[1])); // from A to M

	// cout << ps[0][0] << " " << ps[0][1] << " | " << ps[1][0] << " " << ps[1][1] << " = ";
	// cout << mid[0] << " " << mid[1] << " / " << ortho[0] << " " << ortho[1] << " / " ;
	// cout << schlope << " + " << b << " = " << dp2msq << " ? " << r << "\n";

	if (dp2msq > sq(r)) return std::vector<double>(0);
	double dm2c = std::sqrt(r * r - dp2msq); // from M to the centers

	for (int i = 0; i < 2; i++) {
		int sc = i ? 1 : -1;
		res[2 * i] = mid[0] + sc * dm2c * ortho[0];
		res[2 * i + 1] = mid[1] + sc * dm2c * ortho[1];
	}
	return res;
}
