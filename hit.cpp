#include "sphere.h"
#include "hit.h"

Hit::Hit(double time, const Point3D& position,
    const Vector3D& normal, const Sphere& object)
    :time{time}, position{position}, normal{unit(normal)}, object{object} {
    // make sure normal has length 1
    
}