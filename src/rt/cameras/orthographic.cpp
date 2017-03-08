#include "orthographic.h"

namespace rt {
  OrthographicCamera::OrthographicCamera(
      const Point& center,
      const Vector& forward,
      const Vector& up,
      float scaleX,
      float scaleY
    ): center(center), scaleX(scaleX), scaleY(scaleY) {
    this->forward = forward.normalize();
    this->up = up.normalize();

    sx = cross(this->forward, this->up).normalize();
    sx = sx * (0.5 * scaleX * this->forward.length());

    sy = -(cross(this->forward, sx).normalize());
    sy = sy * (0.5 * scaleY * this->forward.length());
  }

  Ray OrthographicCamera::getPrimaryRay(float x, float y) const{
    return Ray(center + x*sx + y*sy, forward);
  }
}
