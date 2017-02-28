#include "assert.h"
#include "color.h"

namespace rt {
  RGBColor::RGBColor(): r(0.0), g(0.0), b(0.0) {}
  RGBColor::RGBColor(float r, float g, float  b): r(r), g(g), b(b) {}

  RGBColor::RGBColor(const Float4& f4) {
    NOT_IMPLEMENTED;
  }

  RGBColor RGBColor::rep(float v){
    return RGBColor(v,v,v);
  }

  RGBColor RGBColor::operator+(const RGBColor& c) const{
    return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
  }

  RGBColor RGBColor::operator-(const RGBColor& c) const {
    return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
  }

  RGBColor RGBColor::operator*(const RGBColor& c) const {
    return RGBColor(this->r * c.r, this->g * c.g, this->b * c.b);
  }

  bool RGBColor::operator==(const RGBColor& b) const {
    return (this->r == b.r) && (this->g == b.g) && (this->b == b.b);
  }

  bool RGBColor::operator!=(const RGBColor& b) const {
    return (this->r != b.r) || (this->g != b.g) || (this->b != b.b);
  }

  RGBColor RGBColor::clamp() const {
    return RGBColor(
      this->r < 0.0 ? 0 : (this->r > 1.0 ? 1.0 : this->r),
      this->g < 0.0 ? 0 : (this->g > 1.0 ? 1.0 : this->g),
      this->b < 0.0 ? 0 : (this->b > 1.0 ? 1.0 : this->b)
    );
  }

  RGBColor RGBColor::gamma(float gam) const {
    NOT_IMPLEMENTED;
  }

  float RGBColor::luminance() const {
    NOT_IMPLEMENTED;
  }

  RGBColor operator*(float scalar, const RGBColor& b) {
    return RGBColor(scalar * b.r, scalar * b.g, scalar * b.b);
  }

  RGBColor operator*(const RGBColor& a, float scalar) {
    return scalar*a;
  }

  RGBColor operator/(const RGBColor& a, float scalar) {
    return (1.0 / scalar) * a;
  }
}
