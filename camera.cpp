#include "camera.h"
#include "point3d.h"
#include "vector3d.h"
#include <cmath>


Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect)
    :position{position} {
    
    // Get vector from center of camera to the target
    Vector3D v_to_target{target - position};
    // The hor and ver vectors of the camera
    Vector3D u{unit(cross(v_to_target, up))};
    Vector3D v{unit(cross(v_to_target, u))};

    double width{tan(fov / 2) * 2};
    double height{width / aspect};

    upper_left = position - width / 2 * u - height / 2 * v;
    horizontal = u * width;
    vertical = v * height;
}

Ray Camera::compute_ray(double s, double t) const {
    Point3D origin{upper_left + horizontal * s + vertical * t};
    Point3D start_point{position + unit(cross(vertical, horizontal))};
    return Ray(origin, unit(origin - start_point));
}