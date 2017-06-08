#pragma once

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt {
  class Disc : public Solid {
    private:
      Point center;
      Vector normal;
      float radius;

    public:
      Disc() {}
      Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material);

      virtual BBox getBounds() const;
      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
      virtual Point sample() const;
      virtual float getArea() const;
  };
}
