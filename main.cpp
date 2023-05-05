#include "ray.h"

int main() {
    Ray ray{{0, 0, 0}, {0, 1, 0}};
    std::cout << ray << '\n';
    std::cout << ray.at(1) << '\n';
}