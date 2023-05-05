#pragma once

#include <iostream>

class Vector2D {
public:
  Vector2D();
  Vector2D(double x, double y);

  Vector2D& operator+=(const Vector2D& rhs);
  Vector2D& operator-=(const Vector2D& rhs);
  Vector2D& operator*=(const Vector2D& rhs);
  Vector2D& operator*=(const double scalar);
  Vector2D& operator/=(const double scalar);

  double x, y;
};

bool operator==(const Vector2D& lhs, const Vector2D& rhs);
bool operator!=(const Vector2D& lhs, const Vector2D& rhs);

Vector2D operator+(Vector2D lhs, const Vector2D& rhs);
Vector2D operator-(Vector2D lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& rhs); // negation
Vector2D operator*(Vector2D lhs, const Vector2D& rhs);
Vector2D operator*(Vector2D lhs, const double scalar);
Vector2D operator*(const double scalar, Vector2D rhs);
Vector2D operator/(Vector2D lhs, const double scalar);

Vector2D pow(Vector2D v, const double n);
double dot(const Vector2D& a, const Vector2D& b);
Vector2D cross(const Vector2D& a, const Vector2D& b);
double length(const Vector2D& v);
Vector2D unit(const Vector2D& v);

std::ostream& operator<<(std::ostream& os, const Vector2D& v);
std::istream& operator>>(std::istream& is, Vector2D& v);
  

