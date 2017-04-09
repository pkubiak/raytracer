#pragma once

#include <rt/integrators/integrator.h>

namespace rt {
  class World;
  class Ray;
  class RGBColor;

  class RecursiveRayTracingIntegrator : public Integrator {
    public:
      RecursiveRayTracingIntegrator(World* world) : Integrator(world) {}
      virtual RGBColor getRadiance(const Ray& ray) const;
    private:
      RGBColor getRadiance(const Ray& ray, int currentRecursionDepth) const;
      static const int maxRecursionDepth = 6;
  };
}
