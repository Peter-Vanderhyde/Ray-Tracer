#include "camera.h"
#include "point3d.h"
#include "vector3d.h"
#include "constants.h"
#include "random.h"
#include <cmath>

double degree_to_radian(double degrees);

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect, double focus_dist, double blur_strength)
    :position{position}, blur_strength{blur_strength * 0.1} {
    
    // Get vector from center of camera to the target
    Vector3D v_to_target{target - position};
    // The hor and ver vectors of the camera
    u = unit(cross(v_to_target, up));
    v = unit(cross(v_to_target, u));

    double width{tan(degree_to_radian(fov) / 2) * 2};
    double height{width / aspect};

    // Determine how to scale the viewport to account for focus depth
    double dist_to_target = length(v_to_target);
    if (dist_to_target == 0) {
        throw std::runtime_error("The target cannot be the same as the camera position.");
    } else if (focus_dist == 0) {
        throw std::runtime_error("Cannot set focus distance to 0.");
    }

    double scale = dist_to_target * focus_dist;
    upper_left = position - (width / 2) * u * scale - (height / 2) * v * scale;
    upper_left += v_to_target * focus_dist;

    horizontal = u * width * scale;
    vertical = v * height * scale;
}

Ray Camera::compute_ray(double s, double t) const {
    Point3D origin{upper_left + horizontal * s + vertical * t};
    Vector3D rand_dir;
    Point3D start_point = position;
    if (blur_strength != 0) {
        rand_dir = random_in_unit_disk(Vector3D(1, 0, 1)) * blur_strength;
        start_point += u * rand_dir.x + v * rand_dir.z;
    }
    
    return Ray(start_point, unit(origin - start_point));
}

double degree_to_radian(double degrees) {
    return degrees * Constants::Pi / 180;
}