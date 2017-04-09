#pragma once

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {
  class PhongMaterial : public Material {
    protected:
      Texture *specular;
      float exponent;

    public:
      PhongMaterial(Texture* specular, float exponent);
      virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
      virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual Sampling useSampling() const;
  };
}
