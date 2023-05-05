#include "ray.h"
#include "triangle.h"
#include <iostream>

int main() {
    Ray ray{{0, 0, 0}, {0, 1, 0}};

    Triangle triangle{{0, 1, 1}, {1, 1, -1}, {-1, 1, -1}};
    auto time = triangle.intersect(ray);
    if (time.has_value()) {
        std::cout << "Intersects at t=" << time.value() << '\n';
    }
    else {
        std::cout << "No hit\n";
    }
}