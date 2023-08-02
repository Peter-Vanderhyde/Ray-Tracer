#pragma once

#include "vector3d.h"

double random_double(); // [0.0, 1.0)
double random_double(double min, double max); // [min, max)
int random_int(int min, int max);
Vector3D random_unit_vector();
Vector3D random_in_hemisphere(const Vector3D& normal);
Vector3D random_in_unit_disk(Vector3D disk_plane);