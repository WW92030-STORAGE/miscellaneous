#ifndef TEST_FFT_EXT
#define TEST_FFT_EXT

#include "FFT.h"
#include "comp.h"

#define cf(a, b) Complex(a, b)

FFT_NUM randrad() {
    FFT_NUM res = (FFT_NUM)(rand()) / (FFT_NUM)(RAND_MAX);
    return -1 + 2 * res;
}

void test1() {
    srand(1);
    std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	
    vector<Complex> v({cf(1, 0), cf(2, -1), cf(0, -1), cf(-1, 2)});
    
    auto res = DFT(v);
    auto res2 = FFT(v);
    std::cout << disp(res) << "\n";
    std::cout << disp(res2) << "\n";
    
    int N = (1<<15);
    v = vector<Complex>(N);
    for (int i = 0; i < v.size(); i++) v[i] = Complex(randrad() * 32, randrad() * 32);
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

    assert(abs(max_diff_x) < 0.000001);
    assert(abs(max_diff_y) < 0.000001);
}

void functest() {
    // our function is square(x) = sgn(sin(2pi * x / 32))
    // the sampling rate is 1, so each index is +1 x value.
    int N = 128;
    int M = 5;
    std::vector<Complex> inputs(N);
    for (int i = 0; i < N; i++) inputs[i] = Complex(0.1 * i * i - 0.005 * i * i * i + 0.000063 * i * i * i * i, 0);
    for (int i = 0; i < N; i++) inputs[i] = Complex((i % (M<<1) < M) ? 1 : -1, 0);
    // for (int i = 0; i < N; i++) inputs[i] = Complex(cos(0.125 * M_PI * i), sin(0.125 * M_PI * i));

    auto freq = FFT(inputs);

    // sort in descending order by amplitude

    std::vector<std::pair<std::pair<double, int>, Complex>> descending;
    for (int i = 0; i < N; i++) descending.push_back({{-1 * freq[i].lengthsquared(), i}, freq[i]});
    std::sort(descending.begin(), descending.end());

    std::vector<Complex> solution(N);
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N - 2; k++) {
            auto term = descending[k].second;
            double argument = term.length();
            double phase = term.theta();
            double FF = (double)(descending[k].first.second) / N;
            Complex value = frompolar(argument, 2 * M_PI * FF * i + phase);

            solution[i] = solution[i] + value;
        }
    }

    for (int i = 0; i < N; i++) solution[i] = solution[i] * (1.0 / N);

    // for (int i = 0; i < N; i++) cout << inputs[i].to_string() << " | " << solution[i].to_string() << " | " << (solution[i] - inputs[i]).length() << endl;
    for (int i = 0; i < N; i++) assert((solution[i] - inputs[i]).length() < 0.01);
}

#endif