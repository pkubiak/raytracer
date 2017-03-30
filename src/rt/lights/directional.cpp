#include "directional.h"
#include<cmath>

namespace rt {

  DirectionalLight::DirectionalLight(const Vector& _direction, const RGBColor& _color):
    direction(_direction), color(_color) {}

  LightHit DirectionalLight::getLightHit(const Point& p) const {
    LightHit hit;
    hit.direction = direction;
    hit.distance = INFINITY;

    return hit;
  }

  RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return color;
  }
}
