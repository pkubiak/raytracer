#include "world.h"
#include <core/float4.h>
#include <core/assert.h>

namespace rt {
  WorldMapper::WorldMapper() {}
  WorldMapper::WorldMapper(const Float4& _scale): scale(_scale){
    assert(scale[3] == 0.0f);
  };
  Point WorldMapper::getCoords(const Intersection& hit) const {
    Point hp = hit.local();
    return Point(hp.x * scale[0], hp.y * scale[1], hp.z * scale[2]);
  }
}
