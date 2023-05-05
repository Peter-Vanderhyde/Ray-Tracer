#include "hit.h"
#include "vector3d.h"

Hit::Hit(double time, const Point3D& position,
    const Vector3D& normal, Material* material)
    :time{time}, position{position}, normal{unit(normal)}, material{material} {
    // make sure normal has length 1
    
}