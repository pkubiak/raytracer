#include "quad.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../intersection.h"
#include "../ray.h"

namespace rt {
  Quad::Quad(const Point& _v1, const Vector& _span1, const Vector& _span2, CoordMapper* _texMapper, Material* _material):
    Solid(_texMapper, _material), v1(_v1), span1(_span1), span2(_span2) {
      boundingBox = BBox::empty();
      boundingBox.extend(v1);
      boundingBox.extend(v1+span1);
      boundingBox.extend(v1+span2);
      boundingBox.extend(v1+(span1+span2));
    }

  BBox Quad::getBounds() const {
    return boundingBox;
  }

  Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    Vector normal = cross(span1, span2).normalize();
    float t = dot(normal, v1 - ray.o)/dot(ray.d, normal);

    if(t < 0 || t > previousBestDistance)
      return Intersection::failure();

    Vector vp = (ray.o + t*ray.d) - v1;
    float v12 = dot(span1, span2), v1p = dot(span1, vp), v11 = dot(span1, span1), v22 = dot(span2,span2), v2p = dot(span2, vp);

    float u = (v12*v2p - v22*v1p)/(v12*v12 - v11*v22);
    float v = (v12*v1p - v11*v2p)/(v12*v12 - v11*v22);
    // printf("%f %f\n", u, v))
    if(u<0.0 || v < 0.0 || u >1.0 || v > 1.0)
      return Intersection::failure();
    // Vector vvv = (u*span1+v*span2)-vp;
    // printf("%f, %f -> [%f,%f,%f]\n", u,v, vvv.x,vvv.y,vvv.z);
    // if(dot(normal, ray.d) > 0.0)
      // return Intersection(t, ray, this, -normal, Point(u,v,0));
    return Intersection(t, ray, this, normal, Point(u,v,0));
  }

  Point Quad::sample() const {
    NOT_IMPLEMENTED;
  }

  float Quad::getArea() const {
    NOT_IMPLEMENTED;
  }
}
