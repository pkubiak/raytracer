#pragma once

#include <core/vector.h>

namespace rt {
  class Intersection;

  class CoordMapper {
    public:
        virtual Point getCoords(const Intersection& hit) const = 0;
  };
}
