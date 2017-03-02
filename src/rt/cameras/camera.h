#pragma once
#include "../ray.h"

namespace rt {

  class Camera {
  public:
      virtual Ray getPrimaryRay(float x, float y) const = 0;
  };
}
