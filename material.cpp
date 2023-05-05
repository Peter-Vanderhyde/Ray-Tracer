#include "material.h"
#include "color.h"
#include "random.h"
#include "hit.h"
#include <cmath>

Material::Material(std::string name, Color color, bool emitting)
    :name{name}, color{color}, emitting{emitting} {}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Name: " << material.name << '\n';
    os << "Color: " << material.color << '\n';
    os << "Emitting: " << std::boolalpha << material.emitting << '\n';
    return os;
}

Diffuse::Diffuse(Color color, bool emitting)
    :Material("diffuse", color, emitting) {}

Ray Diffuse::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
}

Specular::Specular(Color color, bool emitting)
    :Material("specular", color, emitting) {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal));
}

Metal::Metal(Color color, bool emitting, double fuzz)
    :Material("metal", color, emitting), fuzz{fuzz} {}

Ray Metal::scatter(const Ray& ray, const Hit& hit) const {
    return Ray(hit.position, reflect(ray.direction, hit.normal) + fuzz * random_unit_vector());
}

Vector3D reflect(const Vector3D& vector, const Vector3D& normal) {
    return vector - 2 * dot(vector, normal) * normal;
}