#include "assert.h"
#include "point.h"
#include "vector.h"

namespace rt {
  Point::Point(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

  Point::Point(const Float4& f4) {
    NOT_IMPLEMENTED;
  }

  Point Point::rep(float v) {
    return Point(v,v,v);
  }

  Vector Point::operator-(const Point& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
  }

  bool Point::operator==(const Point& b) const {
    return (this->x == b.x) && (this->y == b.y) && (this->z == b.z);
  }

  bool Point::operator!=(const Point& b) const {
    return (this->x != b.x) || (this->y != b.y) || (this->z != b.z);
  }

  Point operator*(float scalar, const Point& b) {
    return Point(scalar * b.x, scalar * b.y, scalar * b.z);
  }

  Point operator*(const Point& a, float scalar) {
    return scalar * a;
  }

  Point min(const Point& a, const Point& b) {
    return Point(
      a.x < b.x ? a.x : b.x,
      a.y < b.y ? a.y : b.y,
      a.z < b.z ? a.z : b.z
    );
  }
  Point max(const Point& a, const Point& b) {
    return Point(
      a.x > b.x ? a.x : b.x,
      a.y > b.y ? a.y : b.y,
      a.z > b.z ? a.z : b.z
    );
  }

  float Point::operator()(int p) const{
    if(p == 0)return x;
    if(p == 1)return y;
    if(p == 2)return z;
    UNREACHABLE;
  }
}
