#ifndef CG1RAYTRACER_MATERIALS_MIRROR_HEADER
#define CG1RAYTRACER_MATERIALS_MIRROR_HEADER

#include <rt/materials/material.h>

namespace rt {

class MirrorMaterial : public Material {
public:
    MirrorMaterial(float eta, float kappa);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const { return RGBColor::rep(0.0f); }
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const { return RGBColor::rep(0.0f); }
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const { return Material::SAMPLING_ALL; }
};

}

#endif
