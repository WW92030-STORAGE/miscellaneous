#ifndef POINT_H
#define POINT_H

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>

#define PI (std::atan(1) * 4)

struct Point {
    double x = 0;
    double y = 0;
    Point() {
        x = 0;
        y = 0;
    }
    Point(double a, double b) {
        x = a;
        y = b;
    }
    Point(const Point& other) {
        x = other.x;
        y = other.y;
    }
    
    bool operator<(Point& other) {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(Point& other) {
        return (x == other.x) && (y == other.y);
    }
    
    bool operator!=(Point& other) {
        return !(*this == other);
    }
    
    void add(Point other) {
        x += other.x;
        y += other.y;
    }
    
    void sub(Point other) {
        x -= other.x;
        y -= other.y;
    }
    
    void align() {
        x = std::floor(x + 0.5);
        y = std::floor(y + 0.5);
    }
    
    // Rotates the point but also generates a vector of discrete timestamps and the point's position during the rotation.
    // The timestamps include the end point but not the start.
    std::vector<Point> rotate(Point origin, double rad, int steps = 4096) {
        Point vp(*this);
        vp.sub(origin);
        
        Point res(vp.x * std::cos(rad) - vp.y * std::sin(rad), vp.x * std::sin(rad) + vp.y * std::cos(rad));
        x = res.x + origin.x;
        y = res.y + origin.y;
        
        std::vector<Point> retval;
        
        for (int i = 1; i <= steps; i++) {
            double ang = (rad * i) / steps;
            Point p(vp.x * std::cos(ang) - vp.y * std::sin(ang), vp.x * std::sin(ang) + vp.y * std::cos(ang));
            retval.push_back(p);
        }
        return retval;
    }
    
    std::string toString() { return "[" + std::to_string(x) + " " + std::to_string(y) + "]"; }
};

#endif
