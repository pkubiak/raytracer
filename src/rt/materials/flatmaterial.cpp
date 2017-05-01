#include "flatmaterial.h"
#include <core/assert.h>
#include <rt/materials/material.h>
#include <core/point.h>

namespace rt {
  FlatMaterial::FlatMaterial(Texture* _texture): texture(_texture) {}

  RGBColor FlatMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0.0);
  }

  RGBColor FlatMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return texture->getColor(texPoint);
  }

  Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
  }
}
