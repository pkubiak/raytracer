#pragma once

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {
  class FlatMaterial : public Material {
    protected:
      Texture *texture;
    public:
      FlatMaterial(Texture* texture);
      virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
      virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }
  };
}
