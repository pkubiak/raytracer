#pragma once

#include <vector>
#include <rt/groups/group.h>

namespace rt {
  class SimpleGroup : public Group {
    private:
      std::vector<Primitive*> primitives;

    public:
      SimpleGroup(): Group() {}
      virtual BBox getBounds() const;
      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
      virtual void rebuildIndex();
      virtual void add(Primitive* p);
      virtual void setMaterial(Material* m);
      virtual void setCoordMapper(CoordMapper* cm);
  };
}
