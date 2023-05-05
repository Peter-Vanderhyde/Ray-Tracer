#include "vector2d.h"
#include "constants.h"
#include "vector3d.h"
#include <cmath>
#include <iostream>

// JMB: 95/100

Vector2D::Vector2D()
  :x{0.0}, y{0.0} {}

Vector2D::Vector2D(double x, double y)
  :x{x}, y{y} {}

Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& rhs) {
    this->x *= rhs.x;
    this->y *= rhs.y;
    return *this;
}
  
Vector2D& Vector2D::operator*=(const double scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(const double scalar) {
    if (scalar == 0) {
        throw std::overflow_error("Attempted to divide Vector2D by 0!");
    }
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

bool operator==(const Vector2D& lhs, const Vector2D& rhs) {
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y);
}

bool operator!=(const Vector2D& lhs, const Vector2D& rhs) {
    return !(lhs == rhs);
}

Vector2D operator+(Vector2D lhs, const Vector2D& rhs) {
    // JMB: great! You are one of a very small number of students who
    // are relying on previously defined functions.
    return lhs += rhs;
}

Vector2D operator-(Vector2D lhs, const Vector2D& rhs) {
    return lhs -= rhs;
}

Vector2D operator-(const Vector2D& rhs) {
    // JMB: rewrite using operator*
    // return -1 * rhs;
    return rhs * -1;
}

Vector2D operator*(Vector2D lhs, const Vector2D& rhs) {
    return lhs *= rhs;
}

Vector2D operator*(Vector2D lhs, const double scalar) {
    return lhs *= scalar;
}

Vector2D operator*(const double scalar, Vector2D rhs) {
    return rhs *= scalar;
}

Vector2D operator/(Vector2D lhs, const double scalar) {
    return lhs /= scalar;
}

Vector2D pow(Vector2D v, const double n) {
    // JMB: since v is a copy, just update its components: v.x = std::pow(v.x, n);
    v.x = pow(v.x, n);
    v.y = pow(v.y, n);
    return v;
}

double dot(const Vector2D& a, const Vector2D& b) {
    // We will use dot() a lot, so it has to be cheap.
    return a.x * b.x + a.y * b.y;
}

// Vector2D cross(const Vector2D& a, const Vector2D& b) {
//     // JMB: you might want to create "result" with the calculated components for speed.
//     return Vector2D((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
// }

double length(const Vector2D& v) {
    // JMB: the argument for sqrt can be calculated using: dot(v, v)
    return sqrt(dot(v, v));
}

Vector2D unit(const Vector2D& v) {
    // JMB: This is really expensive what you have written. You call
    // length() four times. I would calculate it once, check if it's
    // 0, then return a new, scaled Vector2D.
    double len{length(v)};
    if (len == 0) {
        throw std::overflow_error("Tried to find the unit vector of a vector with no length!");
    }
    return Vector2D(v.x / len, v.y / len);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    return os << "(" << v.x << " " << v.y << ")";
}

std::istream& operator>>(std::istream& is, Vector2D& v) {
    char paren;
    return is >> paren >> v.x >> v.y >> paren;
}
