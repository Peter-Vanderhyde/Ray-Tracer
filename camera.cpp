#include "camera.h"
#include "point3d.h"
#include "vector3d.h"
#include "constants.h"
#include "random.h"
#include <cmath>

double degree_to_radian(double degrees);

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect,
                double aperture, double focus_dist)
    : position(position), lens_radius(aperture / 2.0) {
    
    Vector3D v_to_target = target - position;
    Vector3D u = unit(cross(v_to_target, up));
    Vector3D v = unit(cross(v_to_target, u));

    double height = 2.0 * tan(degree_to_radian(fov / 2.0));
    double width = aspect * height;

    upper_left = position - width / 2.0 * focus_dist * u - height / 2.0 * focus_dist * v + focus_dist * v_to_target;
    horizontal = width * focus_dist * u;
    vertical = height * focus_dist * v;
}

Ray Camera::compute_ray(double s, double t) const {
    Vector3D rd = lens_radius * random_in_unit_disk();
    Vector3D offset = u * rd.x + v * rd.y;

    Point3D origin = position + offset;
    Vector3D direction = upper_left + s * horizontal + t * vertical - origin;

    return Ray(origin, unit(direction));
}

double degree_to_radian(double degrees) {
    return degrees * Constants::Pi / 180;
}