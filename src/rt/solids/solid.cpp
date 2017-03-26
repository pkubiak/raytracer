#include "solid.h"

namespace rt {
  Solid::Solid(CoordMapper* _texMapper, Material* _material):
    material(_material), texMapper(_texMapper) {}
}
