 #include "checkerboard.h"
#include <core/assert.h>
#include <cmath>
#include <core/point.h>

namespace rt {

  CheckerboardTexture::CheckerboardTexture(const RGBColor& _white, const RGBColor& _black): white(_white), black(_black) {}

  RGBColor CheckerboardTexture::getColor(const Point& coord) {
    float x = fmod(coord.x, 1.0f);float y = fmod(coord.y, 1.0f); float z = fmod(coord.z, 1.0f);
    if((x < 0.5f ? 0 : 1) ^ (y < 0.5f ? 0 : 1) ^ (z < 0.5f ? 0 : 1))
      return black;
    return white;
  }

  RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    NOT_IMPLEMENTED;
  }

  RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    NOT_IMPLEMENTED;
  }
}
