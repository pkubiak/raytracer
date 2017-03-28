#include "raytrace.h"
#include <cstdio>
#include <core/color.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>

namespace rt {

  RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = world->scene->intersect(ray);

    if(intersection){
      RGBColor out(0,0,0);

      for(auto light: world->light){
        auto light_hit = light->getLightHit(intersection.hitPoint());

        // Confirm that the shadow ray and the primary ray leave the surface on the same side.
        float x = dot(intersection.normalv, -light_hit.direction);

        if(x < 0)
          intersection.normalv = -intersection.normalv;;

        // If no obstacle is found, query the light for the intensity.
        auto light_intersection = world->scene->intersect(Ray(intersection.ray.o + intersection.ray.d*(intersection.distance-0.00001), -light_hit.direction));

        if(light_intersection && light_intersection.distance < light_hit.distance)
          continue;

        RGBColor intensity = light->getIntensity(light_hit);

        // Use the irradiance intensity and the material properties to compute the amount of reflected light.
        RGBColor reflected = intersection.solid->material->getReflectance(intersection.uv, -intersection.normalv, intersection.ray.d, light_hit.direction);

        // In addition add the emitted light of the material.
        RGBColor emmited = intersection.solid->material->getEmission(intersection.uv, intersection.normalv, intersection.ray.d);

        out.r += intensity.r * reflected.r + emmited.r;
        out.g += intensity.g * reflected.g + emmited.g;
        out.b += intensity.b * reflected.b + emmited.b;
      }
      return out;
    }else
      return RGBColor(0, 0, 0);
  }
}
