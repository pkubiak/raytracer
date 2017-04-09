#include "combine.h"
#include <rt/materials/material.h>
#include <core/assert.h>

namespace rt {
  CombineMaterial::CombineMaterial() {}

  void CombineMaterial::add(Material* material, float weight) {
    materials.push_back(make_pair(material, weight));
  }

  RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor out(0,0,0);

    for(auto p: materials)
      out = out + p.second * p.first->getReflectance(texPoint, normal, outDir, inDir);

    return out;
  }

  RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor out(0,0,0);

    for(auto p: materials)
      out = out + p.second * p.first->getEmission(texPoint, normal, outDir);

    return out;
  }

  Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    for(auto p: materials){
      auto sampling = p.first->useSampling();
      if(sampling == Material::SAMPLING_SECONDARY || sampling == Material::SAMPLING_ALL){
        SampleReflectance res = p.first->getSampleReflectance(texPoint, normal, outDir);
        res.reflectance = res.reflectance * p.second;
        return res;
      }
    }

    UNREACHABLE;
  }

  Material::Sampling CombineMaterial::useSampling() const {
    if(materials.size() == 0)
      return Material::SAMPLING_NOT_NEEDED;

    Sampling res = materials[0].first->useSampling();

    for(auto p: materials) {
      auto sampling = p.first->useSampling();
      if(sampling == Material::SAMPLING_SECONDARY || res != sampling)
        return Material::SAMPLING_SECONDARY;
    }

    return res;
  }
}
