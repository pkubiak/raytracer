#include "disc.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../ray.h"
#include "../intersection.h"

namespace rt {
  Disc::Disc(const Point& _center, const Vector& _normal, float _radius, CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), center(_center), normal(_normal), radius(_radius) {}

  Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    float t = dot(normal, center-ray.o)/dot(ray.d, normal);

    if((t < 0) || (t > previousBestDistance) || (((ray.o + t*ray.d)-center).length() > radius))
      return Intersection::failure();
    return Intersection(t, ray, this, normal, Point());
  }

  BBox Disc::getBounds() const {
    // Bounding box calculation based on: http://iquilezles.org/www/articles/diskbbox/diskbbox.htm
    return BBox(Point(
        center.x - radius * sqrt(1.0 - normal.x*normal.x),
        center.y - radius * sqrt(1.0 - normal.y*normal.y),
        center.z - radius * sqrt(1.0 - normal.z*normal.z)
      ), Point(
        center.x + radius * sqrt(1.0 - normal.x*normal.x),
        center.y + radius * sqrt(1.0 - normal.y*normal.y),
        center.z + radius * sqrt(1.0 - normal.z*normal.z)
      )
    );
  }
  Point Disc::sample() const {
    NOT_IMPLEMENTED;
  }

  float Disc::getArea() const {
    NOT_IMPLEMENTED;
  }
}
