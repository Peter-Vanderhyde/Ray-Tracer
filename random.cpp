#include "random.h"
#include <random>

double random_double() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937_64 engine{rd()};
    static thread_local std::uniform_real_distribution<double> distribution{0.0, 1.0};
    return distribution(engine);
}


double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

Vector3D random_unit_vector() {
    Vector3D p;
    while (true) {
        // Get vector in random direction
        p = {random_double(-1, 1), random_double(-1, 1), random_double(-1, 1)};
        if (dot(p, p) >= 1) { // Ignore the result if the length > 1 to prevent bias
            continue;
        }
        return unit(p);
    }
}

Vector3D random_in_hemisphere(const Vector3D& normal) {
    Vector3D unit_vector{random_unit_vector()};
    if (dot(unit_vector, normal) > 0.0) {
        return unit_vector;
    }
    else {
        return -unit_vector;
    }
}

Vector3D random_in_unit_disk() {
    Vector3D p;
    while (true) {
        p = {random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0};
        if (dot(p, p) >= 1.0) {
            continue;
        }
        return p;
    }
}