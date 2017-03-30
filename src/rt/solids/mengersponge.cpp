#include "mengersponge.h"
#include "parallelepiped.h"

#include <core/assert.h>
#include <rt/intersection.h>
#include "../../core/point.h"
#include <array>
#include <tuple>
using namespace std;

namespace rt {
  MengerSponge::MengerSponge(const Point& corner, const Vector& span1, const Vector& span2, const Vector& span3, int _depth, CoordMapper* texMapper, Material* material):
    Parallelepiped(corner, span1, span2, span3, texMapper, material), depth(_depth){}

  pair<float, Vector> MengerSponge::intersect(const Ray& ray, float previousBestDistance, Point p, Vector v1, Vector v2, Vector v3, int depth) {
    // printf("%d\n", depth);
    if(depth == 0)
      // return make_pair(INFINITY,Vector());
      return Parallelepiped::intersect(ray, previousBestDistance, p, v1,v2,v3);

    pair<float,Vector> best = make_pair(INFINITY, Vector()), ints;

    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
        for(int z=0;z<3;z++){
          if(x*y==1||y*z==1||z*x==1)
            continue;
          ints = Parallelepiped::intersect(ray, previousBestDistance, p+(0.3333333*x)*v1 + (0.3333333*y)*v2 + (0.3333333*z)*v3, (1.0/3.0)*v1, (1.0/3.0)*v2, (1.0/3.0)*v3);
          if(ints.first==INFINITY)
            continue;
          ints = MengerSponge::intersect(ray, previousBestDistance, p+(0.3333333*x)*v1 + (0.3333333*y)*v2 + (0.3333333*z)*v3, (1.0/3.0)*v1, (1.0/3.0)*v2, (1.0/3.0)*v3, depth-1);
          if(ints.first < previousBestDistance){
            previousBestDistance = ints.first;
            best = ints;
          }
        }
    return best;
  }

  Intersection MengerSponge::intersect(const Ray& ray, float previousBestDistance) const {
    pair<float, Vector> res = MengerSponge::intersect(ray, previousBestDistance, p0, v1,v2,v3, depth);
    if(res.first == INFINITY)
      return Intersection::failure();
    return Intersection(res.first, ray, this, res.second, Point());
  }


}
