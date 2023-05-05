#include "camera.h"
#include "point3d.h"
#include "vector3d.h"
#include "constants.h"
#include <cmath>

double degree_to_radian(double degrees);

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect)
    :position{position} {
    
    // Get vector from center of camera to the target
    Vector3D v_to_target{target - position};
    // The hor and ver vectors of the camera
    Vector3D u{unit(cross(v_to_target, up))};
    Vector3D v{unit(cross(v_to_target, u))};
    std::cout << v;

    double width{tan(degree_to_radian(fov) / 2) * 2};
    double height{width / aspect};

    upper_left = position - width / 2 * u - height / 2 * v;
    upper_left += unit(v_to_target);
    horizontal = u * width;
    vertical = v * height;
}

Ray Camera::compute_ray(double s, double t) const {
    Point3D origin{upper_left + horizontal * s + vertical * t};
    return Ray(origin, unit(origin - position));
}

double degree_to_radian(double degrees) {
    return degrees * Constants::Pi / 180;
}