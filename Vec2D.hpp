#ifndef VEC2D_HPP
#define VEC2D_HPP

#include <cmath>
#include <cstdlib>

struct Vec2D {
    double x;
    double y;

    Vec2D(double x, double y) : x(x), y(y) {}
    Vec2D(const Vec2D& v) : x(v.x), y(v.y) {}

    double& operator[](size_t idx);
    const double& operator[](size_t idx) const;

    friend double dot(Vec2D lhs, Vec2D rhs);
    friend double cross_2D(Vec2D lhs, Vec2D rhs);
    friend Vec2D perp(const Vec2D& v);
    friend double distance(Vec2D lhs, Vec2D rhs);
    friend double distance_sqr(Vec2D lhs, Vec2D rhs);

    double angle_rad() const;
    double mag_sqr() const;
    double mag() const;

    friend Vec2D operator+(Vec2D lhs, const Vec2D& rhs);
    friend Vec2D operator-(Vec2D lhs, const Vec2D& rhs);
    friend Vec2D operator*(Vec2D lhs, const double& rhs);
    friend Vec2D operator*(double lhs, const Vec2D& rhs);
    friend Vec2D operator/(Vec2D lhs, const double& rhs);
};

#include "Vec2D.impl"

#endif
