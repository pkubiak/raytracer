#include "bbox.h"
#include<cmath>
#include<algorithm>
#include<utility>
#include "ray.h"
#include "../core/point.h"
#include "../core/vector.h"
#include<cstdio>

namespace rt {
  BBox BBox::empty(){
    return BBox(Point(INFINITY,INFINITY,INFINITY), Point(-INFINITY,-INFINITY,-INFINITY));
  }

  BBox BBox::full(){
    return BBox(Point(-INFINITY,-INFINITY,-INFINITY), Point(INFINITY,INFINITY,INFINITY));
  }

  void BBox::extend(const Point& point) {
    this->min = rt::min(point, this->min);
    this->max = rt::max(point, this->max);
  }

  bool BBox::isEmpty() const {
    return true;

  }
  void BBox::extend(const BBox& bbox){
    if(!bbox.isEmpty()){
      this->extend(bbox.min);
      this->extend(bbox.max);
    }
  }

  std::pair<float,float> BBox::intersect(const Ray& ray) const {
    float t1 = INFINITY, t2 = -INFINITY;

    for(int c=0; c<3; c++)
      for(int s=0;s<2;s++)
        if(ray.d(c) != 0){
          float t = ((s == 0 ? this->min(c) : this->max(c))-ray.o(c))/ray.d(c);
          if(t >= 0){
            Point p = ray.o + (t * ray.d);
            if(((c==0)||(min.x<=p.x&&p.x<=max.x))&&((c==1)||(min.y<=p.y&&p.y<=max.y))&&((c==2)||(min.z<=p.z&&p.z<=max.z))){
              t1 = std::min(t1, t);
              t2 = std::max(t2, t);
            }
          }
        }
    return std::make_pair(t1, t2);
  }

  bool BBox::isUnbound() {
    for(int i=0;i<3;i++)
      if(min(i)==-INFINITY||max(i)==INFINITY)
        return true;
    return false;
  }
}
