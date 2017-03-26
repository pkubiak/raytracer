#include "aabox.h"
#include <core/assert.h>
#include "../intersection.h"
#include <core/point.h>

namespace rt {
  AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material):
    Solid(texMapper, material) {
    min = rt::min(corner1, corner2);
    max = rt::max(corner1, corner2);
  };

  Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    float tbest = INFINITY;
    Vector n;

    for(int c=0; c<3; c++)
      for(int s=0;s<2;s++)
        if(ray.d(c) != 0){
          float t = ((s == 0 ? this->min(c) : this->max(c))-ray.o(c))/ray.d(c);
          if(t >= 0){
            Point p = ray.o + (t * ray.d);
            if(((c==0)||(min.x<=p.x&&p.x<=max.x))&&((c==1)||(min.y<=p.y&&p.y<=max.y))&&((c==2)||(min.z<=p.z&&p.z<=max.z))){
              if(t < tbest){
                tbest = t;
                n = Vector(
                  (2*s-1)*(c==0?1:0),
                  (2*s-1)*(c==1?1:0),
                  (2*s-1)*(c==2?1:0)
                );
              }
            }
          }
        }
    if(tbest <= previousBestDistance)
      return Intersection(tbest, ray, this, n, Point());

    return Intersection::failure();
  }

  BBox AABox::getBounds() const {
    return BBox(min, max);
  }

  Point AABox::sample() const {
    NOT_IMPLEMENTED;
  };

  float AABox::getArea() const {
    NOT_IMPLEMENTED;
  };
}
