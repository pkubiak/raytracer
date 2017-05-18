#pragma once

#include <rt/solids/solid.h>
#include <core/point.h>
#include "../bbox.h"
#include "../ray.h"
#include "../intersection.h"

namespace rt {

class Triangle : public Solid {
  public:
    Point v1;
    Point v2;
    Point v3;
    Vector normal;

    Triangle() {}
    Triangle(Point vertices[3], CoordMapper* texMapper, Material* material);
    Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual Point sample() const;
    virtual float getArea() const;
};

}
