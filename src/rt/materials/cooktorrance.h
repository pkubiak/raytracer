#pragma once

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {
  // Cook-Torrance material for conductors
  class CookTorranceMaterial : public Material {

    protected:
      Texture* texture;
      float eta, kappa, ex, ey;

    public:
      CookTorranceMaterial(Texture* texture, float eta, float kappa, float ex, float ey);

      virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
      virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual Sampling useSampling() const;

  };
}
