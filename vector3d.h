#pragma once

#include <iostream>
#include <vector>

class Vector3D {
public:
  Vector3D();
  Vector3D(double x, double y, double z);

  Vector3D& operator+=(const Vector3D& rhs);
  Vector3D& operator-=(const Vector3D& rhs);
  Vector3D& operator*=(const Vector3D& rhs);
  Vector3D& operator*=(const double scalar);
  Vector3D& operator/=(const double scalar);
  void rotate(const Vector3D &rotations);
  void scale(const Vector3D& scales);
  std::vector<double> val_list() const;

  double x, y, z;
};

bool almost_equal(double x, double y);
bool operator==(const Vector3D& lhs, const Vector3D& rhs);
bool operator!=(const Vector3D& lhs, const Vector3D& rhs);

Vector3D operator+(Vector3D lhs, const Vector3D& rhs);
Vector3D operator-(Vector3D lhs, const Vector3D& rhs);
Vector3D operator-(const Vector3D& rhs); // negation
Vector3D operator*(Vector3D lhs, const Vector3D& rhs);
Vector3D operator*(Vector3D lhs, const double scalar);
Vector3D operator*(const double scalar, Vector3D rhs);
Vector3D operator/(Vector3D lhs, const double scalar);

Vector3D pow(Vector3D v, const double n);
double dot(const Vector3D& a, const Vector3D& b);
Vector3D cross(const Vector3D& a, const Vector3D& b);
double length(const Vector3D& v);
Vector3D unit(const Vector3D& v);

std::ostream& operator<<(std::ostream& os, const Vector3D& v);
std::istream& operator>>(std::istream& is, Vector3D& v);
  
void rotate_values(double& x, double& y, double angle);
double d_to_r(double degrees);