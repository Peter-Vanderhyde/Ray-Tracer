#include "diffuse.h"
#include "random.h"
#include "ray.h"
#include "hit.h"
#include "color.h"

Diffuse::Diffuse(Color color, bool emitting)
    :Material("diffuse", color, emitting) {}

Ray Diffuse::scatter(const Ray&, const Hit& hit) const {
    return Ray(hit.position, random_in_hemisphere(hit.normal));
}