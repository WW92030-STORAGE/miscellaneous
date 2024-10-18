#ifndef RAYLIB_UTILS_H
#define RAYLIB_UTILS_H

#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include <memory>
#include <cmath>
#include <vector>


// Draw an axis aligned rectangle from two opposing points
inline void Rect2Points(double x, double y, double w, double h, Color col) {
    if (x > w || y > h) {
        Rect2Points(std::min(x, w), std::min(y, h), std::max(x, w), std::max(y, h), col);
        return;
    }
    Rectangle rect = {x, y, w - x, h - y};
    DrawRectangleRec(rect, col);
}

// Draw a rotated rectangle parallel to two points and with a specified half-thickness

inline void DrawRotatedRect(double x1, double y1, double x2, double y2, double rad, Color col) {
    if (x1 > x2) {
        DrawRotatedRect(x2, y2, x1, y1, rad, col);
        return;
    }

    double dist = (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1);
    dist = sqrt(dist);
    Rectangle rec = {0.5 * (x1 + x2), 0.5 * (y1 + y2), dist, 2 * rad};
    Vector2 origin = {0.5 * dist, rad};
    double rotation = atan2(y2 - y1, x2 - x1) * RAD2DEG;

    DrawRectanglePro(rec, origin, rotation, col);
}

// Draw a slot (a rectangle with two circles on the ends) of a specified line segment and radius (half thickness)

inline void DrawSlot(double x1, double y1, double x2, double y2, double rad, Color col) {
    DrawCircle(x1, y1, rad, col);
    DrawCircle(x2, y2, rad, col);

    DrawRotatedRect(x1, y1, x2, y2, rad, col);
}

// Rotations

inline std::pair<double, double> rotpd(std::pair<double, double> point, std::pair<double, double> origin, double rad) {
    double dx = point.first - origin.first;
    double dy = point.second - origin.second;
    std::pair<double, double> blip = {dx * cos(rad) - dy * sin(rad), dx * sin(rad) + dy * cos(rad)};
    return {blip.first + origin.first, blip.second + origin.second};
}

inline std::pair<float, float> rotpf(std::pair<float, float> point, std::pair<float, float> origin, double rad) {
    double dx = point.first - origin.first;
    double dy = point.second - origin.second;
    std::pair<double, double> blip = {dx * cos(rad) - dy * sin(rad), dx * sin(rad) + dy * cos(rad)};
    return {float(blip.first + origin.first), float(blip.second + origin.second)};
}

// Convert from pairs and vectors to Vectors

inline Vector2 vec2vd(std::vector<double> v) {
    if (v.size() < 2) return {0, 0};
    return {v[0], v[1]};
}

inline Vector2 vec2vf(std::vector<float> v) {
    if (v.size() < 2) return {0, 0};
    return {v[0], v[1]};
}

inline Vector2 vec2pd(std::pair<double, double> p) {
    Vector2 res = {p.first, p.second};
    return res;
}

inline Vector2 vec2pf(std::pair<float, float> p) {
    Vector2 res = {p.first, p.second};
    return res;
}

inline Vector3 vec3vd(std::vector<double> v) {
    if (v.size() < 3) return {0, 0, 0};
    return {v[0], v[1], v[2]};
}

inline Vector3 vec3vf(std::vector<float> v) {
    if (v.size() < 3) return {0, 0, 0};
    return {v[0], v[1], v[2]};
}

inline Vector4 vec4vd(std::vector<double> v) {
    if (v.size() < 4) return {0, 0, 0, 0};
    return {v[0], v[1], v[2], v[3]};
}

inline Vector4 vec4vf(std::vector<float> v) {
    if (v.size() < 4) return {0, 0, 0, 0};
    return {v[0], v[1], v[2], v[3]};
}

// Convert from Vectors to pairs and vectors

inline std::pair<double, double> pdvec2(Vector2 v) {
    return std::pair<double, double>{v.x, v.y};
}

inline std::pair<float, float> pfvec2(Vector2 v) {
    return std::pair<float, float>{v.x, v.y};
}

inline std::vector<double> vdvec3(Vector3 v) {
    return std::vector<double>({v.x, v.y, v.z});
}

inline std::vector<float> vfvec3(Vector3 v) {
    return std::vector<float>({v.x, v.y, v.z});
}

inline std::vector<double> vdvec4(Vector4 v) {
    return std::vector<double>({v.x, v.y, v.z, v.w});
}

inline std::vector<float> vfvec4(Vector4 v) {
    return std::vector<float>({v.x, v.y, v.z, v.w});
}


#endif
