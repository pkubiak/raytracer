#pragma once

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>

namespace rt {
  class CheckerboardTexture : public Texture {
    public:
      CheckerboardTexture(const RGBColor& white, const RGBColor& black);
      virtual RGBColor getColor(const Point& coord);
      virtual RGBColor getColorDX(const Point& coord);
      virtual RGBColor getColorDY(const Point& coord);
    private:
      RGBColor white;
      RGBColor black;
  };
}
