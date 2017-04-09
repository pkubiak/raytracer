#include "lambertian.h"
#include <rt/materials/material.h>
#include <core/assert.h>

namespace rt {
  LambertianMaterial::LambertianMaterial(Texture* emission_, Texture* diffuse_):
    emission(emission_), diffuse(diffuse_) {}

  RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float c = dot(normal, inDir)/(normal.length()*inDir.length());

    return c*diffuse->getColor(texPoint)/3.1415926;
  }

  RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return emission->getColor(texPoint);
  }

  Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    // return Sample
    NOT_IMPLEMENTED;
  }

  Material::Sampling LambertianMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
  }
}
