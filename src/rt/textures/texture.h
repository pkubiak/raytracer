#pragma once

namespace rt {

  class RGBColor;
  class Point;

  class Texture {
    public:
      virtual RGBColor getColor(const Point& coord) = 0;
      virtual RGBColor getColorDX(const Point& coord) = 0;
      virtual RGBColor getColorDY(const Point& coord) = 0;
      virtual ~Texture() {};
  };

}
