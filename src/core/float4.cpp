#include "float4.h"
#include "point.h"
#include "vector.h"

namespace rt {

  Float4::Float4(float x, float y, float z, float w): v{x,y,z,w} {}
  Float4::Float4(const Point& p): v{p.x, p.y, p.z, 1.0f} {}
  Float4::Float4(const Vector& _v): v{_v.x, _v.y, _v.z, 0.0f} {}

  float& Float4::operator[](int idx){
    return v[idx];
  }

  float Float4::operator[](int idx) const{
    return v[idx];
  }

  Float4 Float4::operator+(const Float4& b) const{
    return Float4(v[0]+b.v[0], v[1]+b.v[1], v[2]+b.v[2], v[3]+b.v[3]);
  }

  Float4 Float4::operator-(const Float4& b) const{
    return Float4(v[0]-b.v[0], v[1]-b.v[1], v[2]-b.v[2], v[3]-b.v[3]);
  }

  Float4 Float4::operator*(const Float4& b) const{
    return Float4(v[0]*b.v[0], v[1]*b.v[1], v[2]*b.v[2], v[3]*b.v[3]);
  }

  Float4 Float4::operator/(const Float4& b) const{
    return Float4(v[0]/b.v[0], v[1]/b.v[1], v[2]/b.v[2], v[3]/b.v[3]);
  }

  Float4 Float4::operator-() const{
    return Float4(-v[0], -v[1], -v[2], -v[3]);
  }

  bool Float4::operator==(const Float4& b) const{
    return (v[0] == b.v[0]) && (v[1] == b.v[1]) && (v[2] == b.v[2]) && (v[3] == b.v[3]);
  }

  bool Float4::operator!=(const Float4& b) const{
    return (v[0] != b.v[1]) || (v[1] != b.v[1]) || (v[2] != b.v[2]) || (v[3] != b.v[3]);
  }

  Float4 operator*(float scalar, const Float4& b) {
      return Float4(scalar*b[0], scalar*b[1], scalar*b[2], scalar*b[3]);
  }

  Float4 operator*(const Float4& a, float scalar) {
    return scalar * a;
  }

  Float4 operator/(const Float4& a, float scalar) {
    return Float4(a[0]/scalar, a[1]/scalar, a[2]/scalar, a[3]/scalar);
  }

  float dot(const Float4& a, const Float4& b) {
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
  }

  Float4 min(const Float4& a, const Float4& b) {
    return Float4(
      a[0] < b[0] ? a[0] : b[0],
      a[1] < b[1] ? a[1] : b[1],
      a[2] < b[2] ? a[2] : b[2],
      a[3] < b[3] ? a[3] : b[3]
    );
  }

  Float4 max(const Float4& a, const Float4& b) {
    return Float4(
      a[0] > b[0] ? a[0] : b[0],
      a[1] > b[1] ? a[1] : b[1],
      a[2] > b[2] ? a[2] : b[2],
      a[3] > b[3] ? a[3] : b[3]
    );
  }


}
