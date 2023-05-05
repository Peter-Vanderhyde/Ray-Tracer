#include "vector3d.h"
#include "constants.h"
#include <cmath>
#include <iostream>

// JMB: 95/100

Vector3D::Vector3D()
  :x{0.0}, y{0.0}, z{0.0} {}

Vector3D::Vector3D(double x, double y, double z)
  :x{x}, y{y}, z{z} {}

Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& rhs) {
    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    return *this;
}
  
Vector3D& Vector3D::operator*=(const double scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator/=(const double scalar) {
    if (scalar == 0) {
        throw std::overflow_error("Attempted to divide Vector3D by 0!");
    }
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
}

bool almost_equal(double x, double y) {
    return std::abs(x - y) < Constants::Epsilon;
}

bool operator==(const Vector3D& lhs, const Vector3D& rhs) {
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(const Vector3D& lhs, const Vector3D& rhs) {
    return !(lhs == rhs);
}

Vector3D operator+(Vector3D lhs, const Vector3D& rhs) {
    // JMB: great! You are one of a very small number of students who
    // are relying on previously defined functions.
    return lhs += rhs;
}

Vector3D operator-(Vector3D lhs, const Vector3D& rhs) {
    return lhs -= rhs;
}

Vector3D operator-(const Vector3D& rhs) {
    // JMB: rewrite using operator*
    // return -1 * rhs;
    return rhs * -1;
}

Vector3D operator*(Vector3D lhs, const Vector3D& rhs) {
    return lhs *= rhs;
}

Vector3D operator*(Vector3D lhs, const double scalar) {
    return lhs *= scalar;
}

Vector3D operator*(const double scalar, Vector3D rhs) {
    return rhs *= scalar;
}

Vector3D operator/(Vector3D lhs, const double scalar) {
    return lhs /= scalar;
}

Vector3D pow(Vector3D v, const double n) {
    // JMB: since v is a copy, just update its components: v.x = std::pow(v.x, n);
    v.x = pow(v.x, n);
    v.y = pow(v.y, n);
    v.z = pow(v.z, n);
    return v;
}

double dot(const Vector3D& a, const Vector3D& b) {
    // JMB: this is a bit expensive, since you allocate a new Vector,
    // then throw it away. I would do:
    // return a.x*b.x + a.y*b.y + a.z*b.z
    
    // We will use dot() a lot, so it has to be cheap.
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D cross(const Vector3D& a, const Vector3D& b) {
    // JMB: you might want to create "result" with the calculated components for speed.
    return Vector3D((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

double length(const Vector3D& v) {
    // JMB: the argument for sqrt can be calculated using: dot(v, v)
    return sqrt(dot(v, v));
}

Vector3D unit(const Vector3D& v) {
    // JMB: This is really expensive what you have written. You call
    // length() four times. I would calculate it once, check if it's
    // 0, then return a new, scaled Vector3D.
    double len{length(v)};
    if (len == 0) {
        throw std::overflow_error("Tried to find the unit vector of a vector with no length!");
    }
    return Vector3D(v.x / len, v.y / len, v.z / len);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    return os << "(" << v.x << " " << v.y << " " << v.z << ")";
}

std::istream& operator>>(std::istream& is, Vector3D& v) {
    char paren;
    return is >> paren >> v.x >> v.y >> v.z >> paren;
}
