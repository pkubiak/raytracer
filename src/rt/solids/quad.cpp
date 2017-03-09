#include "quad.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../intersection.h"
#include "../ray.h"

namespace rt {
  Quad::Quad(const Point& _v1, const Vector& _span1, const Vector& _span2, CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), v1(_v1), span1(_span1), span2(_span2) {
      v2 = v1 + span1;
      v3 = v1+(span1+span2);
      v4 = v1+span2;

      tr1 = new Triangle(v1, v2, v3, _texMapper, _material);
      tr2 = new Triangle(v1, v3, v4, _texMapper, _material);
    }

  BBox Quad::getBounds() const {
    NOT_IMPLEMENTED;
  }

  Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection ins = tr1->intersect(ray, previousBestDistance);
    if(ins)return ins;

    ins = tr2->intersect(ray, previousBestDistance);
    if(ins)return ins;

    return Intersection::failure();

    // float t = dot(normal, v1-ray.o)/dot(ray.d, normal);
    // if(t < 0 || t > previousBestDistance)
    //   return Intersection::failure();

    // o+u*s1+v*s2 = p
    // u*s1.x + v*s2.x = p.x - o.x
    // u*S1.y + v*s2.y = p.y - o.y
    // u*s1.x + v*s2.y*s1.x/s1.y = (p.y - o.y)*s1.x/s1.y

    // Point p = ray.o + t * ray.d;

    //v*(s2.x - s1.x/s1.y) = (p.x - o.x) - (p.y - o.y)*s1.x/s1.y
    // float u = ((p.x - v1.x) - (p.y - v1.y)*span1.x/span1.y) / (span2.x - span1.x/span1.y);
    // float v = (p.x - v1.x - u*span1.x)/span2.x;
    // printf("%f %f\n", u, v);
    // if((u < 0.0) || (u > 1.0) || (v < 0.0) || (v > 0.0))
    //   return Intersection::failure();
    //
    // return Intersection(t, ray, this, normal, Point(u, v, 0));
  }

  Point Quad::sample() const {
    NOT_IMPLEMENTED;
  }

  float Quad::getArea() const {
    NOT_IMPLEMENTED;
  }
}
