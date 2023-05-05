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
    vt0{fmax(vt0.x - 0.0001, 0), fmax(vt0.y - 0.0001, 0)}, vt1{fmax(vt1.x - 0.0001, 0), fmax(vt1.y - 0.0001, 0)},
    vt2{fmax(vt2.x - 0.0001, 0), fmax(vt2.y - 0.0001, 0)} {}

Point2D TriangleObj::uv(const Hit& hit) const {
    Vector3D A{vertex0};
    Vector3D B{vertex1};
    Vector3D C{vertex2};
    Vector3D P = hit.position;
    double barya, baryb, baryc;
    if (A.x == B.x && A.x == C.x) {
        barya = ((B.y - C.y) * (P.z - C.z) + (C.z - B.z) * (P.y - C.y)) /
                ((B.y - C.y) * (A.z - C.z) + (C.z - B.z) * (A.y - C.y));
        baryb = ((C.y - A.y) * (P.z - C.z) + (A.z - C.z) * (P.y - C.y)) /
                ((B.y - C.y) * (A.z - C.z) + (C.z - B.z) * (A.y - C.y));
    }
    else if ((A.y == B.y && A.y == C.y) || ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y) == 0)) {
        barya = ((B.z - C.z) * (P.x - C.x) + (C.x - B.x) * (P.z - C.z)) /
                ((B.z - C.z) * (A.x - C.x) + (C.x - B.x) * (A.z - C.z));
        baryb = ((C.z - A.z) * (P.x - C.x) + (A.x - C.x) * (P.z - C.z)) /
                ((B.z - C.z) * (A.x - C.x) + (C.x - B.x) * (A.z - C.z));
    }
    else {
        barya = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) /
                ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
        baryb = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) /
                ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
    }
    baryc = 1 - barya - baryb;
    Vector2D Puv = barya * vt0 + baryb * vt1 + baryc * vt2;
    return Vector2D{fmod(Puv.x, 1.0), fmod(Puv.y, 1.0)};
}