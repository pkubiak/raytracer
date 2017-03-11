#include "intersection.h"
#include "../core/point.h"
#include <core/assert.h>

namespace rt {

  Intersection Intersection::failure(){
    Intersection res = Intersection();
    res.distance = -INFINITY;
    return res;
  }

  Intersection::Intersection(float _distance, const Ray& _ray, const Solid* _solid, const Vector& _normal, const Point& _uv):
    distance(_distance), ray(_ray), solid(_solid), uv(_uv) {
      normalv = _normal.normalize();
    }

  // Intersection::Intersection(Intersection& other): distance(other.distance), ray(other.ray), solid(other.solid), _normal(other._normal){ }

  Point Intersection::hitPoint() const {
    return ray.o + distance*ray.d;
  }

  Vector Intersection::normal() const {
    return normalv;
  }

  Point Intersection::local() const {
    return uv;
  }

  Intersection::operator bool(){
    return (distance != -INFINITY);
  }
}
