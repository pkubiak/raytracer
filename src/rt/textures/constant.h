#pragma once

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>

namespace rt {
  class ConstantTexture : public Texture {
    protected:
      RGBColor color;
    public:
        ConstantTexture();
        ConstantTexture(const RGBColor& color);
        //TODO: Why not const?
        virtual RGBColor getColor(const Point& coord);
        virtual RGBColor getColorDX(const Point& coord);
        virtual RGBColor getColorDY(const Point& coord);
  };
}
