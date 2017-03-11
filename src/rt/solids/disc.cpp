#include "disc.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../ray.h"
#include "../intersection.h"

namespace rt {
  Disc::Disc(const Point& _center, const Vector& _normal, float _radius, CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), center(_center), normal(_normal), radius(_radius) {
    // printf("[%f, %f, %f]", normal.x, normal.y, normal.z);
  }

  BBox Disc::getBounds() const {
    NOT_IMPLEMENTED;
  }

  Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    float t = dot(normal, center-ray.o)/dot(ray.d, normal);
    // printf("[%f, %f,  %f]", normal.x, normal.y, normal.z);
    // printf("[%f,%f,%f] [%f,%f,%f]\n", ray.d.x, ray.d.y, ray.d.z, normal.x, normal.y,normal.z);
    if((t < 0) || (t > previousBestDistance) || (((ray.o + t*ray.d)-center).length() > radius))
      return Intersection::failure();
    return Intersection(t, ray, this, normal, Point());
  }

  Point Disc::sample() const {
    NOT_IMPLEMENTED;
  }

  float Disc::getArea() const {
    NOT_IMPLEMENTED;
  }
}
