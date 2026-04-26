#ifndef COMP_EXT
#define COMP_EXT

#include <algorithm>
#include <string>
#include <cfloat>
#include <cstdint>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// change me????
#define FFT_NUM double

struct Complex {
    FFT_NUM x = 0;
    FFT_NUM y = 0;
    
    Complex() {
        x = 0;
        y = 0;
    }
    
    Complex(FFT_NUM a, FFT_NUM b) {
        x = a;
        y = b;
    }
    
    Complex operator+(const Complex& other) {
        return Complex(x + other.x, y + other.y);
    }
    
    Complex operator-(const Complex& other) {
        return Complex(x - other.x, y - other.y);
    }
    
    Complex operator*(FFT_NUM f) {
        return Complex(x * f, y * f);
    }
    
    Complex operator*(const Complex& other) {
        return Complex(x * other.x - y * other.y, x * other.y + y * other.x);
    }
    
    Complex conjugate() {
        return Complex(x, -1 * y);
    }

    FFT_NUM lengthsquared() {
        return x * x + y * y;
    }

    FFT_NUM length() {
        return sqrt(lengthsquared());
    }

    FFT_NUM theta() {
        return atan2(y, x);
    }

    bool operator<(const Complex& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    std::string to_string() {
        return std::to_string(x) + " + " + std::to_string(y) + "i";
    }
};

Complex frompolar(FFT_NUM r, FFT_NUM theta) {
    constexpr FFT_NUM tau = 2 * M_PI;
    if (theta > tau || theta < -tau) theta = std::remainder(theta, tau);
    return Complex(r * cos(theta), r * sin(theta));
}

Complex oncircle(FFT_NUM theta) {
    constexpr FFT_NUM tau = 2 * M_PI;
    if (theta > tau || theta < -tau) theta = std::remainder(theta, tau);
    return Complex(cos(theta), sin(theta));
}

Complex uroot(int64_t n, int64_t i) {
    constexpr FFT_NUM tau = (2 * M_PI);
    if (i > n || i < -n) i %= n;
    return oncircle((tau / (FFT_NUM)(n)) * i);
}

std::vector<Complex> uroots(int64_t n) {
    constexpr FFT_NUM tau = 2 * M_PI;
    FFT_NUM spacing = tau / (FFT_NUM)(n);
    std::vector<Complex> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = oncircle(spacing * i);
    }
    return v;
}

#endif