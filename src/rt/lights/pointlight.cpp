#include "pointlight.h"

namespace rt {
  PointLight::PointLight(){}

  PointLight::PointLight(const Point& _position, const RGBColor& _intensity): position(_position), intensity(_intensity) {}

  LightHit PointLight::getLightHit(const Point& p) const {
    LightHit hit;
    hit.direction = p-position;
    hit.distance = hit.direction.length();
    hit.direction = hit.direction.normalize();

    return hit;
  };

  RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return intensity/(irr.distance * irr.distance);
  };
}
