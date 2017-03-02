#include "ray.h"
#include<cstdio>

namespace rt {
  Ray::Ray(const Point& o, const Vector& d): origin(o) {
    this->direction = d.normalize();
    // printf("%f\n", this->direction.length());
  }

  Point Ray::getPoint(float distance) const {
    return origin + (distance * direction);
  }
}
