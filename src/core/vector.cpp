#include<cmath>
#include "assert.h"
#include "vector.h"
#include "point.h"

namespace rt {
  Vector::Vector(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

  Vector::Vector(const Float4& f4) {
    NOT_IMPLEMENTED;
  }

  Vector Vector::rep(float v) {
    return Vector(v,v,v);
  }

  Vector Vector::operator+(const Vector& b) const {
    return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
  }

  Vector Vector::operator-(const Vector& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
  }

  Vector Vector::normalize() const {
    float length = this->length();
    if(length == 0.0)
      return Vector();
    return Vector(this->x / length, this->y / length, this->z / length);
  }

  Vector Vector::operator-() const {
    return Vector(-this->x, -this->y, -this->z);
  }

  float Vector::lensqr() const {
    return this->x * this->x + this->y * this->y + this->z * this->z;
  }

  float Vector::length() const {
    return sqrt(this->lensqr());
  }

  bool Vector::operator==(const Vector& b) const {
    return (this->x == b.x) && (this->y == b.y) && (this->z == b.z);
  }

  bool Vector::operator!=(const Vector& b) const {
    return (this->x != b.x) || (this->y != b.y) || (this->z != b.z);
  }


  Vector operator*(float scalar, const Vector& b) {
    return Vector(scalar * b.x, scalar * b.y, scalar * b.z);
  }

  Vector operator*(const Vector& a, float scalar) {
    return scalar * a;
  }

  Vector operator/(const Vector& a, float scalar) {
    return Vector(a.x / scalar, a.y / scalar, a.z / scalar);
  }

  Vector cross(const Vector& a, const Vector& b) {
    return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
  }

  float dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  Vector min(const Vector& a, const Vector& b) {
    return Vector(
      a.x < b.x ? a.x : b.x,
      a.y < b.y ? a.y : b.y,
      a.z < b.z ? a.z : b.z
    );
  }
  Vector max(const Vector& a, const Vector& b) {
    return Vector(
      a.x > b.x ? a.x : b.x,
      a.y > b.y ? a.y : b.y,
      a.z > b.z ? a.z : b.z
    );
  }

  Vector reflect(const Vector& n, const Vector& v){
    return (2.0f*dot(n, v)/n.length())*n - v;
  }

  Point operator+(const Point& a, const Vector& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  Point operator+(const Vector& a, const Point& b) {
    return b + a;
  }

  Point operator-(const Point& a, const Vector& b) {
    return a + (-b);
  }

  Point operator*(const Float4& scale, const Point& p) {
    NOT_IMPLEMENTED;
  }



  const float Vector::operator()(int p) const{
    return *(&x+p);
  }

}
