#pragma once
#include "ray.h"
#include "point3d.h"

class Vector3D;

class Camera {
public:
    Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect, double focus_dist, double blur_strength);

    // s, t are normalized values [0, 1] that describe the position along the horizontal,
    // vertical directions in the camera's coordinate system
    Ray compute_ray(double s, double t) const;

private:
    Point3D position;
    Point3D upper_left;
    double blur_strength;
    Vector3D u, v;
    Vector3D horizontal, vertical; // Vectors along u, v with lengths width, height
};