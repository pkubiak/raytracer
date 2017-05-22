#include "projective.h"

#include <core/point.h>
#include <core/color.h>
#include <core/vector.h>

#include <rt/lights/light.h>
#include <rt/textures/texture.h>

#include <cmath>
#include <cstdio>

namespace rt {

  ProjectiveLight::ProjectiveLight(const Point& position_, const Vector& direction_, const Vector& up_, Texture* texture_, float angle_, float factor_):
    position(position_), direction(direction_), up(up_), texture(texture_), angle(angle_), factor(factor_) {
      left = cross(up, direction);
      up = cross(left, direction).normalize();
      direction = direction.normalize();
      left = left.normalize();
    }

  LightHit ProjectiveLight::getLightHit(const Point& p) const {
    LightHit hit;

    hit.direction = p-position;
    hit.distance = hit.direction.length();
    hit.direction = hit.direction.normalize();

    return hit;
  }

  RGBColor ProjectiveLight::getIntensity(const LightHit& irr) const {
    Vector dir = irr.direction;

    float a = dot(dir, direction)/(dir.length()*direction.length());
    a = std::acos(a)/(0.5*angle);

    auto vz = direction*(dot(dir, direction)/direction.lensqr());
    float d = std::acos(dot(dir-vz, up)/(up.length() * (dir-vz).length()));

    if (dot(cross(up,direction), dir) > 0.0)
      d = 2.0*M_PI - d;

    // scale to [0,1]
    float x = 0.5*(-a*sin(d)+1.0);
    float y = 0.5*(a*cos(d)+1.0);

    return factor*texture->getColor(Point(x,y,0))/(irr.distance * irr.distance);
  }
}
