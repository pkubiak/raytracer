#include "cooktorrance.h"
#include <rt/materials/material.h>
#include <rt/textures/texture.h>
#include <core/assert.h>
#include <cmath>

/*
  Reference:
    An Anisotropic Phong Light Reflection Model, Michael Ashikhmin & Peter Shirley
    http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.18.4504&rep=rep1&type=pdf
*/
namespace rt {
  CookTorranceMaterial::CookTorranceMaterial(Texture* _texture, float _eta, float _kappa, float _ex, float _ey):
    texture(_texture), eta(_eta), kappa(_kappa), ex(_ex), ey(_ey) {}

  RGBColor CookTorranceMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    // inDir = inDir.normalize();
    // outDir = outDir.normalize();
    Vector halfDir = (inDir + outDir).normalize();
    // normal = normal.normalize();
    // printf("inDir: [%f, %f, %f]\n", inDir.x, inDir.y, inDir.z);
    // printf("outDir: [%f, %f, %f]\n", outDir.x, outDir.y, outDir.z);
    // printf("normal: [%f, %f, %f]\n", normal.x, normal.y, normal.z);
    // // printf("normal: [%f, %f, %f]\n", normal.x, normal.y, normal.z);

    assert(abs(inDir.length() - 1.0f) < 0.000001f);
    assert(abs(outDir.length() - 1.0f) < 0.000001f);
    assert(abs(normal.length() - 1.0f) < 0.000001f);

    // Microfacet distribution
    // TODO: Fix bug vvvvvvvvv we must calculate vectors u, v
    float fi = acos(dot(halfDir, normal));
    printf("%f\n", fi);
    float e = ex*cos(fi)*cos(fi) + ey*sin(fi)*sin(fi);

    float D = sqrt((ex+2)*(ey+2)/(2.0*M_PI))*pow(dot(halfDir, normal), e);

    // Geometric attenuation factor
    float G = 1.0;

    float G1 = 2.0f*dot(normal, halfDir)*dot(normal, outDir)/dot(outDir, halfDir);
    float G2 = 2.0f*dot(normal, halfDir)*dot(normal, inDir)/dot(inDir, halfDir);

    if(G1 < G)G = G1;
    if(G2 < G)G = G2;

    // Fresnel term
    float cos_o = dot(normal, outDir)/(normal.length() * outDir.length());

    float term1 = (eta*eta+kappa*kappa), term2 = 2*eta*cos_o, term3 = cos_o*cos_o;

    float r_parallel = 1.0 - 2.0*term2/(term1*term3 + term2 + 1.0);
    float r_perpendicular = 1.0 - 2.0*term2/(term1 + term2 + term3);

    float F = 0.5*(r_parallel + r_perpendicular);

    //Final coefficient
    // printf("%f %f %f\n", D,G,F);

    float cos_ = dot(normal, inDir);

    float c = cos_*D*G*F/(4.0f*dot(normal, outDir)*dot(normal, inDir));


    return c*texture->getColor(texPoint);
  }

  RGBColor CookTorranceMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
  }

  Material::SampleReflectance CookTorranceMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
  }

  Material::Sampling CookTorranceMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
  }
}
