#include "cylindrical.h"
#include<cmath>
#include <core/point.h>
#include <rt/intersection.h>
#include <cstdio>

namespace rt {

  CylindricalCoordMapper::CylindricalCoordMapper(const Point& _origin, const Vector& _longitudinalAxis, const Vector& _polarAxis):
    origin(_origin), longitudinalAxis(_longitudinalAxis), polarAxis(_polarAxis) {}

  Point CylindricalCoordMapper::getCoords(const Intersection& hit) const{
    Point hp = hit.local();
    Vector v = hp - origin;
    float y = dot(v, longitudinalAxis)/longitudinalAxis.lensqr();

    Vector direction = hp - (origin + y*longitudinalAxis);

    Vector vec = polarAxis - longitudinalAxis*(dot(longitudinalAxis, polarAxis)/longitudinalAxis.lensqr());

    float cosa = dot(direction, vec)/(vec.length() * direction.length());
    if(cosa<-1.0f)cosa=-1.0f;
    if(cosa>1.0f)cosa=1.0f;
    float x = std::acos(cosa);
    if(dot(cross(direction, vec), longitudinalAxis) < 0.0)
      x = 2.0*M_PI-x;

    x = (0.5*x/M_PI)/polarAxis.length();

    return Point(x, y, 0);
  }
}
