#include "assert.h"
#include "color.h"

namespace rt {
  RGBColor::RGBColor(): r(0.0), g(0.0), b(0.0) {}
  RGBColor::RGBColor(float _r, float _g, float  _b): r(_r), g(_g), b(_b) {}

  RGBColor::RGBColor(const Float4& f4) {
    NOT_IMPLEMENTED;
  }

  RGBColor RGBColor::rep(float v){
    return RGBColor(v,v,v);
  }

  RGBColor RGBColor::operator+(const RGBColor& c) const{
    return RGBColor(r + c.r, g + c.g, b + c.b);
  }

  RGBColor RGBColor::operator-(const RGBColor& c) const {
    return RGBColor(r - c.r, g - c.g, b - c.b);
  }

  RGBColor RGBColor::operator*(const RGBColor& c) const {
    return RGBColor(r * c.r, g * c.g, b * c.b);
  }

  bool RGBColor::operator==(const RGBColor& c) const {
    return (r == c.r) && (g == c.g) && (b == c.b);
  }

  bool RGBColor::operator!=(const RGBColor& c) const {
    return (r != c.r) || (g != c.g) || (b != c.b);
  }

  RGBColor RGBColor::clamp() const {
    return RGBColor(
      r < 0.0 ? 0 : (r > 1.0 ? 1.0 : r),
      g < 0.0 ? 0 : (g > 1.0 ? 1.0 : g),
      b < 0.0 ? 0 : (b > 1.0 ? 1.0 : b)
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
