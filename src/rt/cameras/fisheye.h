#pragma once

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {
  class FishEyeCamera : public Camera {
    public:
      Point center;
      Vector forward;
      float openingAngle;

      FishEyeCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float openingAngle
      );

      virtual Ray getPrimaryRay(float x, float y) const;

    private:
      Vector fx, fy;
  };
}
