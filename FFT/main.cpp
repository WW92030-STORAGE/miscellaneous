#include <bits/stdc++.h>
using namespace std;

#include "FFT.h"

#define cf(a, b) Complex(a, b)

int main()
{
    std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	
    vector<Complex> v({cf(1, 0), cf(2, -1), cf(0, -1), cf(-1, 2)});
    
    auto res = DFT(v);
    auto res2 = FFT(v);
    std::cout << disp(res) << "\n";
    std::cout << disp(res2) << "\n";
    
    int N = ((1<<13) - 1)<<1;
    v = vector<Complex>(N);
    for (int i = 0; i < v.size(); i++) v[i] = Complex((287352 * i + 2309752) % 17, (289753 * i + 2935) & 19);
    res = DFT(v);
    
	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
	
	start = std::chrono::high_resolution_clock::now();
    
    res2 = FFT(v);
    
	end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;
	
    double min_diff_x = 1;
    double min_diff_y = 1;

    double max_diff_x = 0;
    double max_diff_y = 0;
	for (int i = 0; i < v.size(); i++) {
        double dx = abs(res[i].x - res2[i].x);
        double dy = abs(res[i].y - res2[i].y);
	    // std::cout << res[i].to_string() << " | " << res2[i].to_string() << "\n";
	    // std::cout <<dx << " " << dy << "\n";
        min_diff_x = min(dx, min_diff_x);
        min_diff_y = min(dy, min_diff_y);
        max_diff_x = max(dx, max_diff_x);
        max_diff_y = max(dy, max_diff_y);
	}

    std::cout << "min " << min_diff_x << " " << min_diff_y << "\n";
    std::cout << "max " << max_diff_x << " " << max_diff_y << "\n";
	
    return 0;
}