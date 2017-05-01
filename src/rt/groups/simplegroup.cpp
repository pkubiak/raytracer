#include "simplegroup.h"
#include "../bbox.h"
#include "../intersection.h"
#include "../../core/assert.h"

namespace rt {
  BBox SimpleGroup::getBounds() const {
    return bbox;
  }

  Intersection SimpleGroup::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection best = Intersection::failure();
    for(auto p: primitives){
      // printf("%d\n", p);
      auto intersection = p->intersect(ray, previousBestDistance);
      if(intersection){
        best = Intersection(intersection.distance, intersection.ray, intersection.solid, intersection.normal(), intersection.local());
        previousBestDistance = best.distance;
      }
    }
    return best;
  }

  void SimpleGroup::rebuildIndex() {
    // Do nothing
  }

  void SimpleGroup::add(Primitive* p){
    primitives.push_back(p);
    bbox.extend(p->getBounds());
  }

  void SimpleGroup::setMaterial(Material* m) {
    for(auto p: primitives)
      p->setMaterial(m);
  }

  void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    NOT_IMPLEMENTED;
  }
}
