#include "movingperspective.h"
#include "perspective.h"
#include "../ray.h"
#include<cmath>
#include<cstdio>

namespace rt {
  MovingPerspectiveCamera::MovingPerspectiveCamera(
    Point (*_position)(float),
    Point (*_look_at)(float),
    Vector (*_up)(float),
    float _scaleX,
    float _scaleY
  ): scaleX(_scaleX), scaleY(_scaleY), position(_position), look_at(_look_at), up(_up), camera(nullptr) {
    setTime(0.0);
  }

  void MovingPerspectiveCamera::setTime(float _t) {
    t = _t;
    if(camera != nullptr)
      delete camera;

    Point center = position(t);
    Vector direction = look_at(t)-center;
    Vector up_dir = up(t);
    camera = new PerspectiveCamera(center, direction, up_dir, scaleX, scaleY);
  }

  // x,y in [-1, 1]
  Ray MovingPerspectiveCamera::getPrimaryRay(float x, float y) const {
    return camera->getPrimaryRay(x, y);
  }
}
