#include "material.h"
#include "color.h"
#include "random.h"
#include "hit.h"
#include "texture.h"
#include <cmath>

Material::Material(std::string name)
    :name{name}, index_ratio{1.0} {}

Material::Material(std::string name, double index_ratio)
    :name{name}, index_ratio{index_ratio} {}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Name: " << material.name << '\n';
    return os;
}

Light::Light()
    :Material("light") {
    emitting = true;
}

Ray Light::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, ray.direction);
}

Diffuse::Diffuse()
    :Material("diffuse") {}

Ray Diffuse::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
}

Specular::Specular()
    :Material("specular") {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal));
}

Metal::Metal(double fuzz)
    :Material("metal"), fuzz{fuzz} {}

Ray Metal::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal) + fuzz * random_unit_vector());
}

Glass::Glass(double index_ratio)
    :Material("glass", index_ratio) {}

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

FlatGlass::FlatGlass(double index_ratio, double thickness)
    :Material("flat_glass", index_ratio), thickness{thickness} {}

Ray FlatGlass::scatter(const Ray& ray, const Hit& hit) const {
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

        return Ray{hit.position + unit(refracted) * thickness, direction};
    }
}

Gloss::Gloss(double roughness, double fuzz)
    :Material("gloss"), roughness{roughness}, fuzz{fuzz} {}

Ray Gloss::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered;

    double ran = random_double(0, 1);
    if (ran > roughness) {
        scattered = reflect(unit(ray.direction), hit.normal) + fuzz * random_unit_vector();
    }
    else {
        scattered = random_in_hemisphere(hit.normal);
    }

    return Ray(hit.position, scattered);
}

Foggy::Foggy()
    :Material("foggy") {}

Ray Foggy::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_unit_vector());
}

PointLight::PointLight()
    :Material("point_light") {
    emitting = true;
}

Ray PointLight::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
}

DirectionalLight::DirectionalLight(double spread_angle)
    :Material("directional_light") {
    emitting = true;
    spread = fmin(fabs(spread_angle), 90.0);
}

Ray DirectionalLight::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
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

double mix(double glossy, double diffuse) {
    double mixFactor = std::clamp(glossy, 0.0, 1.0);
    
    // Mix the two materials together
    return mixFactor * glossy + (1.0 - mixFactor) * diffuse;
}