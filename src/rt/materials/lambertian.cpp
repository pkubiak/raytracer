#include "lambertian.h"
#include <rt/materials/material.h>
#include <core/assert.h>
#include <cmath>

namespace rt {
  LambertianMaterial::LambertianMaterial(Texture* emission_, Texture* diffuse_):
    emission(emission_), diffuse(diffuse_) {}

  RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float c = dot(normal, inDir)/(normal.length()*inDir.length());

    return c*diffuse->getColor(texPoint)/M_PI;
  }

  RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return emission->getColor(texPoint);
  }

  Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
  }

  Material::Sampling LambertianMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
  }
}
