#include "material.h"
#include "color.h"

Material::Material(std::string name, Color color, bool emitting)
    :name{name}, color{color}, emitting{emitting} {}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Name: " << material.name << '\n';
    os << "Color: " << material.color << '\n';
    os << "Emitting: " << std::boolalpha << material.emitting << '\n';
    return os;
}