#include "casting.h"
#include <core/assert.h>
#include <core/color.h>
#include "../intersection.h"
#include "../world.h"

namespace rt {
  RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    Intersection ints = this->world->scene->intersect(ray);

    if(ints){
      Vector n = ints.normal();
      Vector d = ray.d;
      float radiance = fabs(dot(n, d)/(n.length()*d.length()));

      return RGBColor::rep(radiance);
    }else
      return RGBColor(0, 0, 0);
  }
}
