#pragma once

#include <rt/materials/material.h>

namespace rt {
  class MirrorMaterial : public Material {
    protected:
      float eta, kappa;

    public:
      MirrorMaterial(float eta, float kappa);
      virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const { return RGBColor::rep(0.0f); }
      virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const { return RGBColor::rep(0.0f); }
      virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual Sampling useSampling() const { return Material::SAMPLING_ALL; }
  };
}
