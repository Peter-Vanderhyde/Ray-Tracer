#include "material.h"
#include "color.h"
#include "random.h"
#include "hit.h"
#include "texture.h"
#include <cmath>

Material::Material(std::string name, bool emitting)
    :name{name}, emitting{emitting}, index_ratio{1.0} {}

Material::Material(std::string name, bool emitting, double index_ratio)
    :name{name}, emitting{emitting}, index_ratio{index_ratio} {}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Name: " << material.name << '\n';
    os << "Emitting: " << std::boolalpha << material.emitting << '\n';
    return os;
}

Diffuse::Diffuse(bool emitting)
    :Material("diffuse", emitting) {}

Ray Diffuse::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
}

Specular::Specular(bool emitting)
    :Material("specular", emitting) {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal));
}

Metal::Metal(bool emitting, double fuzz)
    :Material("metal", emitting), fuzz{fuzz} {}

Ray Metal::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal) + fuzz * random_unit_vector());
}

Glass::Glass(bool emitting, double index_ratio)
    :Material("glass", emitting, index_ratio) {}

Ray Glass::scatter(const Ray& ray, const Hit& hit) const {
    auto direction = ray.direction;
    auto normal = hit.normal;
    double n1{ray.index_ratio};
    double n2{index_ratio};

    if (dot(direction, normal) > 0) {
        normal *= -1;
        std::swap(n1, n2);
    }

    double cos_theta = -dot(direction, normal); // Angle of ray coming into surface
    double probability = schlick(cos_theta, n1 / n2);

    if (random_double() < probability) {
        auto reflected = reflect(direction, normal);
        return Ray{hit.position, reflected};
    }
    else {
        auto refracted = refract(direction, normal, n1 / n2);
        return Ray{hit.position, refracted};
    }
}

Gloss::Gloss(bool emitting)
    :Material("gloss", emitting) {}

Ray Gloss::scatter(const Ray& ray, const Hit& hit) const {
    if (random_double() < 0.35) {
        return Ray{hit.position, reflect(ray.direction, hit.normal)};
    }
    else {
        return Ray(hit.position, random_in_hemisphere(hit.normal));
    }
}

Foggy::Foggy()
    :Material("foggy", false) {}

Ray Foggy::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_unit_vector());
}

Vector3D reflect(const Vector3D& vector, const Vector3D& normal) {
    return vector - 2 * dot(vector, normal) * normal;
}

Vector3D refract(const Vector3D &vector, const Vector3D &normal, double index_ratio) {
    double cos_theta = -dot(normal, vector);
    double discriminant = 1.0 - std::pow(index_ratio, 2) * (1 - std::pow(cos_theta, 2));
    if (discriminant >= 0) {
        // return refract
        return index_ratio * vector + (index_ratio * cos_theta - std::sqrt(discriminant)) * normal;
    }
    else { // total internal reflection
        return reflect(vector, normal);
    }
}

double schlick(double cos_theta, double index_ratio) {
    double r0 = std::pow((1 - index_ratio) / (1 + index_ratio), 2);
    return r0 + (1 - r0) * std::pow(1 - cos_theta, 5);
}