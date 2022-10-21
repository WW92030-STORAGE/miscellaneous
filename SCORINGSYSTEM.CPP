#include <bits/stdc++.h> 
using namespace std;

// Simulation thing to be used on collected data

int sgn(double x) {
    return (x >= 0) ? 1 : -1;
}

double ln(double x) {
    if (x < 0) return ln(abs(x));
    if (x < 1) return 0;
    return log(x);
}

int main()
{
	int v, pc, nc, im, h, r;
	double score = 0;
	int n = 1;
	
	for (int i = 0; i < n; i++) {
	    cin >> v >> pc >> nc >> im >> h >> r;
	    double res = 4 * ln(v) * sgn(v) + 8 * (pc - 2 * nc) + 2 * ln(im) * sgn(im) + 4 * ln(h + 1) + ln(r + 1);
	    score += res;
	    cout << res << " " << score << endl;
	}
	
   	return 0;
}
