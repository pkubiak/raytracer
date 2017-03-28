#pragma once

#include <rt/primitive.h>
namespace rt {
  class Light;
  class World {
  public:
      Primitive* scene;
  	typedef std::vector<Light*> LightVector;
      LightVector light;

  };
}
