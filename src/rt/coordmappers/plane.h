#pragma once

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>
#include <core/matrix.h>
#include <rt/intersection.h>

namespace rt {
  class Intersection;

  class PlaneCoordMapper : public CoordMapper{
    protected:
      // Vector v1, v2, n;
      Matrix m;

    public:
      PlaneCoordMapper(const Vector& e1, const Vector& e2);
      virtual Point getCoords(const Intersection& hit) const;
  };
}
