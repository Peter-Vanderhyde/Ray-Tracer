#include "shape.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"

Shape::Shape(Material* material, Texture* texture)
    :material{material}, texture{texture} {}