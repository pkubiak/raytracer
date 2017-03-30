#include "spotlight.h"

namespace rt {
  SpotLight::SpotLight(const Point& _position, const Vector& _direction, float _angle, float _exp, const RGBColor& _intensity):
    PointLight(_position, _intensity), angle(_angle), exp(_exp), direction(_direction){};

  RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    float a = dot(irr.direction, direction);
    if(a < 0)
      return RGBColor(0,0,0);
    a = a*a/(irr.direction.lensqr()*direction.lensqr());
    if(a < cos(angle)*cos(angle))
      return RGBColor(0,0,0);

    a = pow(a, 0.5*exp);
    return a*intensity/(irr.distance*irr.distance);
  }
}
