#include "tmapper.h"
#include <rt/intersection.h>

namespace rt {

  TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2):
    v0(tv0), v1(tv1), v2(tv2) {};

  TriangleMapper::TriangleMapper(Point ntv[3]):
    v0(ntv[0]), v1(ntv[1]), v2(ntv[2]) {};

  Point TriangleMapper::getCoords(const Intersection& hit) const {
    const Point p = hit.local();

    return Point(
      v0.x*p.x + v1.x*p.y + v2.x*p.z,
      v0.y*p.x + v1.y*p.y + v2.y*p.z,
      v0.z*p.x + v1.z*p.y + v2.z*p.z
    );
  }

}
