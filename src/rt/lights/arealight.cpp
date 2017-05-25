#include "arealight.h"
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <core/color.h>
#include <rt/materials/material.h>
#include <cstdio>
#include <rt/solids/quad.h>

namespace rt {
  AreaLight::AreaLight(Solid* source_): source(source_){}

  LightHit AreaLight::getLightHit(const Point& p) const {
    Point sample = source->sample();

    LightHit hit;
    hit.direction = p-sample;
    hit.distance = hit.direction.length();
    hit.direction = hit.direction.normalize();

    return hit;
  }

  // TODO: this method only support LambertianMaterial with ConstantTexture
  RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    float area = source->getArea();
    RGBColor radiance = source->material->getEmission(Point(0,0,0), Vector(0,0,0), Vector(0,0,0));

    return area*radiance/(irr.distance * irr.distance);
  }

}
