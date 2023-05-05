#include "triangleobj.h"
#include "ray.h"
#include "constants.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "point2d.h"
#include "triangle.h"

TriangleObj::TriangleObj(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
    const Point2D& vt0, const Point2D& vt1, const Point2D& vt2,
    Material* material, Texture* texture)
    :Triangle{vertex0, vertex1, vertex2, material, texture},
    vt0{fmod(1.0 + vt0.x, 1.0), fmod(1.0 + vt0.y, 1.0)}, vt1{fmod(1.0 + vt1.x, 1.0), fmod(1.0 + vt1.y, 1.0)},
    vt2{fmod(1.0 + vt2.x, 1.0), fmod(1.0 + vt2.y, 1.0)} {}

Point2D TriangleObj::uv(const Hit &hit) const {
    Vector3D A{vertex0};
    Vector3D B{vertex1};
    Vector3D C{vertex2};
    Vector3D P = hit.position;
    double barya = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) /
                    ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
    double baryb = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) /
                    ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
    double baryc = 1 - barya - baryb;
    Vector2D Puv = barya * vt0 + baryb * vt1 + baryc * vt2;
    return Puv;
}