#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>
#include "triangle.h"


namespace rt {

class Quad : public Solid {
private:
  Point v1;
  Vector span1;
  Vector span2;
  // Vector normal;
  Triangle *tr1, *tr2;
  BBox boundingBox;

public:
    // Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual Point sample() const;
    virtual float getArea() const;
};

}

#endif
