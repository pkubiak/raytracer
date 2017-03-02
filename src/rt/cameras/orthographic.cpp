#include "orthographic.h"

namespace rt {
  OrthographicCamera::OrthographicCamera(
      const Point& center,
      const Vector& forward,
      const Vector& up,
      float scaleX,
      float scaleY
    ): center(center), forward(forward), up(up), scaleX(scaleX), scaleY(scaleY) {
    fx = cross(forward, up).normalize();

    fx = fx * (scaleX * forward.length());

    fy = -(cross(forward, fx).normalize());
    fy = fy * (scaleY * forward.length());
  }

  Ray OrthographicCamera::getPrimaryRay(float x, float y) const{
    return Ray(center + x*fx + y*fy, forward);
  }
}
