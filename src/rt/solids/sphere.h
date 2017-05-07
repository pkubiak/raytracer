#pragma once

#include <rt/solids/solid.h>
#include <core/point.h>
#include "../bbox.h"
namespace rt {

class Sphere : public Solid {
  private:
    Point center;
    float radius;

  public:
    Sphere() {}
    Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual Point sample() const;
    virtual float getArea() const;
};

}
