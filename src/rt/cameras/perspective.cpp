#include "perspective.h"
#include "../ray.h"
#include<cmath>
#include<cstdio>

namespace rt {
  PerspectiveCamera::PerspectiveCamera(
    const Point& center,
    const Vector& forward,
    const Vector& up,
    float verticalOpeningAngle,   //in radians
    float horizonalOpeningAngle   //in radians
  ): center(center), forward(forward), up(up),
    verticalOpeningAngle(verticalOpeningAngle),
    horizonalOpeningAngle(horizonalOpeningAngle) {

    // utworz wektory wodzące pola widzenia
    fx = cross(forward, up).normalize();
    
    fx = fx * (tan(0.5*horizonalOpeningAngle)*forward.length());

    fy = -(cross(forward, fx).normalize());
    fy = fy * (tan(0.5*verticalOpeningAngle)*forward.length());
  }

  // x,y in [-1, 1]
  Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector vxy = (x * this->fx) + (y * this->fy);

    return Ray(center, forward + vxy);
  }
}
