#include "phong.h"
#include <rt/materials/material.h>
#include <rt/textures/texture.h>
#include <cmath>
#include <core/assert.h>

namespace rt{
  PhongMaterial::PhongMaterial(Texture* specular_, float exponent_):
    specular(specular_), exponent(exponent_){}

  RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    //asume vector are normalized
    Vector rOutDir = (2.0f*dot(normal, outDir))*normal - outDir;
    float ang = dot(inDir, rOutDir);

    float cos_ = dot(normal, inDir);
    float c = (exponent+2.0f)/(2.0f*M_PI)*(float)pow(std::max(0.0f, ang), exponent)*cos_;

    return c * specular->getColor(texPoint);
  }

  RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
  }

  Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
  }

  Material::Sampling PhongMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
  }
}
