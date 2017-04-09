#include "recraytrace.h"
#include <core/color.h>
#include <rt/materials/material.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <cstdio>

namespace rt {
  RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray, int currentRecursionDepth) const {
    if(currentRecursionDepth <= 0)
      return RGBColor(0, 0, 0);

    Intersection intersection = world->scene->intersect(ray);

    if(intersection){
      Material *material = intersection.solid->material;
      RGBColor out = material->getEmission(intersection.local(), intersection.normal(), intersection.ray.d);

      Material::Sampling sampling = material->useSampling();
      // printf("sampling: %d, %d, %d\n", sampling == Material::SAMPLING_NOT_NEEDED, sampling == Material::SAMPLING_SECONDARY, sampling == Material::SAMPLING_ALL);

      // cast secondary ray
      if((sampling == Material::SAMPLING_ALL) || (sampling == Material::SAMPLING_SECONDARY)){
        // printf("Casting\n");
        // float x = dot(intersection.normalv, intersection.ray.d);

        // if(x < 0)
        //   intersection.normalv = -intersection.normalv;;

        Material::SampleReflectance sample = material->getSampleReflectance(intersection.local(), intersection.normal(), intersection.ray.d);

        RGBColor recurence = getRadiance(
          Ray(intersection.ray.o + intersection.ray.d*(intersection.distance-0.00001), -sample.direction),
          currentRecursionDepth - 1
        );

        out = out + sample.reflectance*recurence;
      }

      // iterate over light sources
      if(sampling == Material::SAMPLING_SECONDARY || sampling == Material::SAMPLING_NOT_NEEDED){
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

          out = out + intensity*reflected;
        }
      }
      return out;
    }else
      return RGBColor(0, 0, 0);
  }

  RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
    return getRadiance(ray, RecursiveRayTracingIntegrator::maxRecursionDepth);
  }
}
