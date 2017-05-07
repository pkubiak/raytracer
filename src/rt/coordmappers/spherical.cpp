#include "spherical.h"
#include <rt/intersection.h>
#include <cstdio>

namespace rt {

  SphericalCoordMapper::SphericalCoordMapper() {

  }

  SphericalCoordMapper::SphericalCoordMapper(const Point& _origin, const Vector& _zenith, const Vector& _azimuthRef):
    origin(_origin), zenith(_zenith) {
      azimuthRef = _azimuthRef - zenith*(dot(_zenith, _azimuthRef)/zenith.lensqr());
      x_scale = 1.0/_azimuthRef.length();
      y_scale = 1.0/_zenith.length();
    }

  Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector dir = hit.local() - origin;

    float y = dot(zenith, dir)/zenith.lensqr();

    Vector vec = dir - y*zenith;

    float x = std::acos(dot(azimuthRef, vec)/(azimuthRef.length() * vec.length()));

    if(dot(cross(vec, azimuthRef), zenith) < 0.0)
      x = 2.0*M_PI - x;


    x = x_scale * (x / (2.0*M_PI));
    y = y_scale * (std::acos(y*zenith.length()/dir.length())/M_PI);

    return Point(x,y,0);
  }
}
