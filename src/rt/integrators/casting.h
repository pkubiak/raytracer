#pragma once

#include <rt/integrators/integrator.h>

namespace rt {
  class World;
  class Ray;
  class RGBColor;

  class RayCastingIntegrator : public Integrator {
    public:
      RayCastingIntegrator(World* world) : Integrator(world) {}
      virtual RGBColor getRadiance(const Ray& ray) const;
  };
}
