#pragma once

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {
  class TriangleMapper : public CoordMapper {
    protected:
      Point v0, v1, v2;
    public:
      TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2);
      TriangleMapper(Point ntv[3]);
      virtual Point getCoords(const Intersection& hit) const;
  };
}
