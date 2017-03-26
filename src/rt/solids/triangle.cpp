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

  Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    // compute t for plane containing triangle v1,v2,v3
    float t0 = dot(normal, v1-ray.o)/dot(ray.d, normal);

    if(t0 < 0 || t0 > previousBestDistance)
      return Intersection::failure();

    // compute barycentric coordinates
    Vector u(v2-v1), v(v3-v1), w((ray.o + t0*ray.d)-v1);
    float uv = dot(u,v), wv = dot(w,v), vv = dot(v,v), wu = dot(w,u), uu = dot(u,u);

    float s = (uv*wv-vv*wu)/(uv*uv-uu*vv);
    float t = (uv*wu-uu*wv)/(uv*uv-uu*vv);


    if(s<0.0 || t < 0.0 || s+t > 1.0)
      return Intersection::failure();

    return Intersection(t0, ray, this, normal, Point(1-s-t,s,t));
  }

  BBox Triangle::getBounds() const {
    BBox bbox = BBox::empty();
    bbox.extend(v1);
    bbox.extend(v2);
    bbox.extend(v3);
    return bbox;
  }

  Point Triangle::sample() const {
    NOT_IMPLEMENTED;
  }

  float Triangle::getArea() const {
    return 0.5*cross(v2-v1, v3-v1).length();
  }
}
