#pragma once

#include <rt/materials/material.h>
#include <rt/materials/combine.h>
#include <vector>
using namespace std;

namespace rt {
  class CombineMaterial : public Material {
    protected:
      vector<pair<Material*, float> > materials;

    public:
      CombineMaterial();
      void add(Material* material, float weight);
      virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
      virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
      virtual Sampling useSampling() const;
  };
}
