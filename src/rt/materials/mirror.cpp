#include "mirror.h"
#include <rt/materials/material.h>
#include <cstdio>

namespace rt {
  MirrorMaterial::MirrorMaterial(float eta_, float kappa_):
    eta(eta_), kappa(kappa_) {}


  Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    Vector inDir = reflect(normal, outDir);

    float cos_o = dot(normal, outDir)/(normal.length() * outDir.length());

    float term1 = (eta*eta+kappa*kappa), term2 = 2*eta*cos_o, term3 = cos_o*cos_o;

    float r_parallel = 1.0 - 2.0*term2/(term1*term3 + term2 + 1.0);
    float r_perpendicular = 1.0 - 2.0*term2/(term1 + term2 + term3);

    float fr = 0.5*(r_parallel + r_perpendicular);

    return Material::SampleReflectance(
      inDir,
      RGBColor::rep(fr)
    );
  };

}
