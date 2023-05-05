#include "shape.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "normal.h"

Shape::Shape(Material* material, Texture* texture, Normal* normal_map)
    :material{material}, texture{texture}, normal_map{normal_map} {}