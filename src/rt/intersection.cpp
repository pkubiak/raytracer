#include "intersection.h"
#include "../core/point.h"
#include <core/assert.h>

namespace rt {

  Intersection Intersection::failure(){
    Intersection res = Intersection();
    res.distance = -INFINITY;
    return res;
  }

  Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv):
    distance(distance), ray(ray), solid(solid), _normal(normal) {
      _normal = _normal.normalize();
    }

  // Intersection::Intersection(Intersection& other): distance(other.distance), ray(other.ray), solid(other.solid), _normal(other._normal){ }

  Point Intersection::hitPoint() const {
    return ray.o + distance*ray.d;
  }

  Vector Intersection::normal() const {
    return _normal;
  }

  Point Intersection::local() const {
    NOT_IMPLEMENTED;
  }

  Intersection::operator bool(){
    return (distance != -INFINITY);
  }
}
