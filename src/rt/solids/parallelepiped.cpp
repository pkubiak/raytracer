#include "parallelepiped.h"
#include <core/assert.h>
#include <rt/intersection.h>
#include "../../core/point.h"
#include <array>
#include <tuple>
using namespace std;

namespace rt {
  Parallelepiped::Parallelepiped(const Point& corner, const Vector& span1, const Vector& span2, const Vector& span3, CoordMapper* texMapper, Material* material):
    Solid(texMapper, material), p0(corner), v1(span1), v2(span2), v3(span3) {

  }

  BBox Parallelepiped::getBounds() const{
    BBox box = BBox::empty();
    box.extend(p0);
    box.extend(p0+v1);
    box.extend(p0+v2);
    box.extend(p0+v3);
    box.extend(p0+(v1+v2));
    box.extend(p0+(v1+v3));
    box.extend(p0+(v2+v3));
    box.extend(p0+(v1+v2+v3));
    return box;
  }

  pair<float, Vector> Parallelepiped::intersect(const Ray& ray, float previousBestDistance, Point p, Vector v1, Vector v2, Vector v3){
    Point q = p+(v1+v2+v3);
    std::array<tuple<Point,Vector, Vector>, 6> quads = {make_tuple(p,v1,v2), make_tuple(p, v1,v3), make_tuple(p,v2,v3), make_tuple(q,-v1,-v2), make_tuple(q,-v1,-v3), make_tuple(q,-v2,-v3)};

    Vector span1,span2;

    float bestT = INFINITY;Vector bestNormal;//Point bestPoint;
    for(auto quad: quads){
      std::tie(p, span1,span2) = quad;

      Vector normal = cross(span1, span2).normalize();
      float t = dot(normal, p - ray.o)/dot(ray.d, normal);

      if(t < 0 || t > previousBestDistance)
        continue;

      Vector vp = (ray.o + t*ray.d) - p;
      float v12 = dot(span1, span2), v1p = dot(span1, vp), v11 = dot(span1, span1), v22 = dot(span2,span2), v2p = dot(span2, vp);

      float u = (v12*v2p - v22*v1p)/(v12*v12 - v11*v22);
      float v = (v12*v1p - v11*v2p)/(v12*v12 - v11*v22);

      if(u<0.0 || v < 0.0 || u >1.0 || v > 1.0)
        continue;

      if(t < bestT){
        bestT = t;bestNormal = normal;//bestPoint=Point(u,v,0);
      }
    }
    // if(bestT<INFINITY)
    return make_pair(bestT, bestNormal);
      // return Intersection(bestT, ray, this, bestNormal, bestPoint);
    // return Intersection::failure();
    // return make_pair(-INFINITY, )
  }

  Intersection Parallelepiped::intersect(const Ray& ray, float previousBestDistance) const{
    pair<float, Vector> res = Parallelepiped::intersect(ray, previousBestDistance, p0, v1, v2, v3);
    if(res.first == INFINITY)
      return Intersection::failure();
    return Intersection(res.first, ray, this, res.second, Point());
  }

  Point Parallelepiped::sample() const {
      NOT_IMPLEMENTED;
  }

  float Parallelepiped::getArea() const {
    NOT_IMPLEMENTED;
  }
}
