#pragma once

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {
  class Intersection;

  class CylindricalCoordMapper : public CoordMapper {
    protected:
      Point origin;
      Vector longitudinalAxis;
      Vector polarAxis;

    public:
      CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
      virtual Point getCoords(const Intersection& hit) const;
  };
}
