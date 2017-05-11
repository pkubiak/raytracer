#include "striangle.h"
#include <rt/intersection.h>
#include <cstdio>

namespace rt {

  SmoothTriangle::SmoothTriangle(Point vertices[3], Vector _normals[3], CoordMapper* texMapper, Material* material):
    Triangle(vertices, texMapper, material), normals({_normals[0].normalize(), _normals[1].normalize(), _normals[2].normalize()}) {};

  SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3,
      const Vector& n1, const Vector& n2, const Vector& n3,
      CoordMapper* texMapper, Material* material):
    Triangle(v1,v2,v3,texMapper, material), normals({n1.normalize(),n2.normalize(),n3.normalize()}) {

    }

  Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const{
    Intersection intr = Triangle::intersect(ray, previousBestDistance);

    if(intr)
      intr.normalv = (
        (intr.local().x * normals[0]) +
        (intr.local().y * normals[1]) +
        (intr.local().z * normals[2])).normalize();

    return intr;
  };
}
