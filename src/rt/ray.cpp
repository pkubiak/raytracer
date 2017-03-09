#include "ray.h"
#include<cstdio>

namespace rt {
  Ray::Ray(const Point& _o, const Vector& _d): o(_o) {
    d = _d.normalize();
  }

  Point Ray::getPoint(float distance) const {
    return o + (distance * d);
  }
}
