#include "ray.h"
#include<cstdio>

namespace rt {
  Ray::Ray(const Point& o, const Vector& d): o(o) {
    this->d = d.normalize();
  }

  Point Ray::getPoint(float distance) const {
    return o + (distance * d);
  }
}
