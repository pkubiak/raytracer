#pragma once

#include <rt/solids/solid.h>
#include <rt/bbox.h>
#include <algorithm>

namespace rt {

  class Parallelepiped : public Solid {
    protected:
      Point p0;
      Vector v1,v2,v3;

    public:
      Parallelepiped() {}
      Parallelepiped(const Point& corner, const Vector& span1, const Vector& span2, const Vector& span3, CoordMapper* texMapper, Material* material);

      virtual BBox getBounds() const;
      static  std::pair<float, Vector> intersect(const Ray& ray, float previousBestDistance, Point p, Vector v1, Vector v2, Vector v3);
      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
      virtual Point sample() const;
      virtual float getArea() const;
  };

}
