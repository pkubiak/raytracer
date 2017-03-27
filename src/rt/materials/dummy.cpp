#include "dummy.h"
#include <core/assert.h>
#include <rt/materials/material.h>

namespace rt {
  DummyMaterial::DummyMaterial() {}

  RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float c = dot(normal, inDir)/(normal.length()*inDir.length());
    return RGBColor(c,c,c);
  }

  RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor(0.0, 0.0, 0.0);
  }

  Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    NOT_IMPLEMENTED;
  }

}
