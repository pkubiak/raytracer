#include "plane.h"
#include <core/point.h>
#include <core/vector.h>
#include <core/assert.h>
#include <core/matrix.h>

namespace rt {
  PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    m = Matrix::system(e1,e2,cross(e1,e2)).invert();
  }

  Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    return (Point)(m*hit.local());
  }
}
