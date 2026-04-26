#ifndef FFT_EXT
#define FFT_EXT

#include <vector>
#include <cstdint>
#include <cfloat>
#include <climits>
#include <string>

#include "comp.h"

#include <iostream>

std::string disp(std::vector<Complex> v) {
    std::string res = "[";
    
    for (uint64_t i = 0; i < v.size(); i++) {
        if (i) res += ", ";
        res += v[i].to_string();
    }
    return res + "]";
}

std::vector<Complex> DFT(std::vector<Complex>& x) {
    std::vector<Complex> res;
    int64_t N = x.size();
    if (N <= 1) return x;
    
    std::vector<Complex> circle = uroots(N);
    res = std::vector<Complex>(N);
    
    for (int64_t k = 0; k < N; k++) {
        for (int64_t n = 0; n < N; n++) {
            int64_t index = -1 * k * n;
            index %= N;
            while (index < 0) index += N;
            Complex value = x[n] * circle[index];
            res[k] = res[k] + value;
        }
    }
    
    return res;
}

std::vector<Complex> FFT(std::vector<Complex>& x) {
    int64_t N = x.size();
    if (N & 1) return DFT(x);
    
    std::vector<Complex> res(N);
    
    std::vector<Complex> odd(N >> 1);
    std::vector<Complex> eve(N >> 1);
    for (int64_t i = 0; i < N; i++) {
        if (i & 1) odd[i>>1] = x[i];
        else eve[i>>1] = x[i];
    }

    
    auto E = FFT(eve);
    auto O = FFT(odd);

    for (int64_t k = 0; k < (N>>1); k++) {
        Complex coeff = uroot(N, -1 * k);
        res[k] = E[k] + (coeff * O[k]);
        res[k + (N>>1)] = E[k] - (coeff * O[k]);
    }
    
    return res;
}

#endif


/*

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
    
    v = vector<Complex>(1<<12);
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
    return 0;
}

*/