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
        best = Intersection(intersection.distance, intersection.ray, intersection.solid, intersection.normal(), Point());
        previousBestDistance = best.distance;
      }
    }
    return best;
  }

  void SimpleGroup::rebuildIndex() {
    NOT_IMPLEMENTED;
  }

  void SimpleGroup::add(Primitive* p){
    primitives.push_back(p);
    bbox.extend(p->getBounds());
  }

  void SimpleGroup::setMaterial(Material* m) {
    NOT_IMPLEMENTED;
  }

  void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    NOT_IMPLEMENTED;
  }
}
