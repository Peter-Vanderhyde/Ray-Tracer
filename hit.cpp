#include "hit.h"
#include "vector3d.h"
#include "shape.h"
#include "sphere.h"
#include "material.h"
#include "texture.h"
#include "shape.h"

Hit::Hit(double time, const Point3D& position, Vector3D normal, const Shape* shape)
    :time{time}, position{position}, normal{normal}, shape{shape} {
    // make sure normal has length 1
    uv = shape->uv(this);
}