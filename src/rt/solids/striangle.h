#pragma once

#include <rt/solids/solid.h>
#include <rt/solids/triangle.h>

namespace rt {
  class SmoothTriangle : public Triangle {
    protected:
      Vector normals[3];
    public:
      SmoothTriangle() {}
      SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material);
      SmoothTriangle(const Point& v1, const Point& v2, const Point& v3,
          const Vector& n1, const Vector& n2, const Vector& n3,
          CoordMapper* texMapper, Material* material);

      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
  };
}
