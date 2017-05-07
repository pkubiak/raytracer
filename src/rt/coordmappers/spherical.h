#pragma once

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {
  class Intersection;
  class SphericalCoordMapper : public CoordMapper {
    protected:
      Point origin;
      Vector zenith, azimuthRef;
      float x_scale, y_scale;

    public:
      SphericalCoordMapper();
      SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef);
      virtual Point getCoords(const Intersection& hit) const;
  };
}
