#pragma once

#include <rt/integrators/integrator.h>

namespace rt {
  class World;
  class Ray;
  class RGBColor;

  class RayTracingIntegrator : public Integrator {
    public:
      RayTracingIntegrator(World* _world): Integrator(_world){};
      virtual RGBColor getRadiance(const Ray& ray) const;
  };
}
