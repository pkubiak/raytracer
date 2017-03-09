#include "infiniteplane.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../intersection.h"
#include "../../core/point.h"

namespace rt {

  InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material):
    Solid(texMapper, material), origin(origin), normal(normal) {

  }

  BBox InfinitePlane::getBounds() const {
    return BBox::full();
  }

  Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    //(ray.o + t*ray.d - origin)*normal == 0
    // (ray.o.x + t*ray.d.x - origin.x)*normal.x+
    // (ray.o.y + t*ray.d.y - origin.y)*normal.y+
    // (ray.o.z + t*ray.d.z - origin.z)*normal.z  = 0
    // t*dot(ray.d, normal) + dot(ray.o, normal) - dot(origin, normal) = 0

    float t = dot(normal, origin-ray.o)/dot(ray.d, normal);
    if(t < 0 || t > previousBestDistance)
      return Intersection::failure();

    return Intersection(t, ray, this, normal, Point());
  }

  Point InfinitePlane::sample() const {
    NOT_IMPLEMENTED;
  }
  float InfinitePlane::getArea() const {
    NOT_IMPLEMENTED;
  }

}
