#include "sphere.h"
#include <core/assert.h>
#include "../bbox.h"
#include "../intersection.h"

namespace rt {

  Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material):
    Solid(texMapper, material), center(center), radius(radius){}

  Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {
    // (p-c)^2-r^2=0
    // p = o+t*d
    // p^2-2*dot(p,c)+c^2-r^2=0
    // (o+td)^2
    // A*t^2 + B*t + C = 0

    //(p - c)^2  -r ^2 = (o+t*d-c)^2 = ((o-c)+t*d)^2 = (o-c)^2 + 2*t*d*(o-c) + t^2*d^2 - r^2 =

    float A = dot(ray.d, ray.d), B = 2*dot(ray.d, ray.o-center), C= dot(ray.o-center, ray.o-center) - radius * radius;


    float delta = B*B - 4*A*C;

    if(delta >= 0.0){
      float t1 = 0.5*(sqrt(delta) - B)/A, t2 = 0.5*(-sqrt(delta) - B)/A;

      if(t2 < 0.0){
        if(t1 > 0.0 && t1 <= previousBestDistance){
          Vector v = (ray.o + t2*ray.d) -center;
          return Intersection(t1, ray, this, v, Point(v.x, v.y, v.z));
        }
      }else if(t2 <= previousBestDistance){
        Vector v = (ray.o + t2*ray.d) -center;
        return Intersection(t2, ray, this, v, Point(v.x, v.y, v.z));
      }
    }

    return Intersection::failure();
  }

  BBox Sphere::getBounds() const {
    return BBox(
      Point(center.x-radius, center.y-radius, center.z-radius),
      Point(center.x+radius, center.y+radius, center.z+radius)
    );
  }
  Point Sphere::sample() const {
    NOT_IMPLEMENTED;
  }

  float Sphere::getArea() const {
    return 4.0*3.1415926*radius*radius;
  }
}
