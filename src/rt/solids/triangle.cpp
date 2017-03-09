#include "triangle.h"
#include <core/assert.h>

namespace rt {
  Triangle::Triangle(Point vertices[3], CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), v1(vertices[0]), v2(vertices[1]), v3(vertices[2]) {
    normal = cross(v2-v1, v3-v1).normalize();
  }

  Triangle::Triangle(const Point& _v1, const Point& _v2, const Point& _v3, CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), v1(_v1), v2(_v2), v3(_v3){
    normal = cross(v2-v1, v3-v1).normalize();
  }

  BBox Triangle::getBounds() const {
    NOT_IMPLEMENTED;
  }

  Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    // compute t for plane containing triangle v1,v2,v3
    float t = dot(normal, v1-ray.o)/dot(ray.d, normal);

    if(t < 0 || t > previousBestDistance)
      return Intersection::failure();

    // compute barycentric coordinates
    Point p = ray.o + t*ray.d;
    float S = cross(v2-v1, v3-v1).length();
    float S1 = cross(v2-p, v3-p).length();
    float S2 = cross(v1-p, v3-p).length();
    float S3 = cross(v1-p, v2-p).length();

    // printf("%f,%f,%f\n", S1/S,S2/S,S3/S);
    //TODO: better condition
    if(S1+S2+S3>S+0.00001)
      return Intersection::failure();
    if(dot(normal, ray.d)>0.0)
      return Intersection(t, ray, this, -normal, Point(S1/S,S2/S,S3/S));
    return Intersection(t, ray, this, normal, Point(S1/S,S2/S,S3/S));
  }

  Point Triangle::sample() const {
    NOT_IMPLEMENTED;
  }

  float Triangle::getArea() const {
    NOT_IMPLEMENTED;
  }
}
