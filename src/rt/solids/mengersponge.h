#pragma once

#include <rt/solids/solid.h>
#include "parallelepiped.h"
#include <rt/bbox.h>
#include <algorithm>

namespace rt {

  class MengerSponge : public Parallelepiped {
    protected:
      int depth;

    public:
      MengerSponge() {}
      MengerSponge(const Point& corner, const Vector& span1, const Vector& span2, const Vector& span3, int depth, CoordMapper* texMapper, Material* material);

      // virtual BBox getBounds() const;
      static  std::pair<float, Vector> intersect(const Ray& ray, float previousBestDistance, Point p, Vector v1, Vector v2, Vector v3, int depth);
      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
      // virtual Point sample() const;
      // virtual float getArea() const;
  };

}
