#pragma once

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/intersection.h>
#include <core/float4.h>

namespace rt {
  class WorldMapper : public CoordMapper {
    protected:
      Float4 scale;

    public:
      WorldMapper();
      explicit WorldMapper(const Float4& scale);
      virtual Point getCoords(const Intersection& hit) const;
  };
}
