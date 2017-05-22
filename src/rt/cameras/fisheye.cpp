#include "fisheye.h"
#include <rt/ray.h>
#include <core/point.h>
#include <core/vector.h>
#include <cmath>

namespace rt {
  FishEyeCamera::FishEyeCamera(
    const Point& center_,
    const Vector& forward_,
    const Vector& up_,
    float openingAngle_
  ): center(center_), forward(forward_), openingAngle(openingAngle_) {
    forward = forward.normalize();

    // utworz wektory wodzące pola widzenia
    fx = cross(forward, up_).normalize();
    // fx = fx * (tan(0.5*horizonalOpeningAngle)*forward.length());

    fy = -(cross(forward, fx).normalize());
    // fy = fy * (tan(0.5*verticalOpeningAngle)*forward.length());
  }

  Ray FishEyeCamera::getPrimaryRay(float x, float y) const {
    float d = openingAngle * sqrt(x*x+y*y)/sqrt(2);
    float a = atan2(x,y);

    // printf("%f\n", sqrt(x*x+y*y));

    auto dir = (cos(a)*fy + sin(a)*fx).normalize();

    auto dir2 = cos(d)*forward + sin(d)*dir;

    return Ray(center, dir2.normalize());
  }
}
