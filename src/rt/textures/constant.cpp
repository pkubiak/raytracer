#include "constant.h"

namespace rt {
  ConstantTexture::ConstantTexture(){}
  ConstantTexture::ConstantTexture(const RGBColor& color_): color(color_){}

  RGBColor ConstantTexture::getColor(const Point& coord){
    return color;
  }

  RGBColor ConstantTexture::getColorDX(const Point& coord){
    return RGBColor::rep(0);
  }

  RGBColor ConstantTexture::getColorDY(const Point& coord){
    return RGBColor::rep(0);
  }
}
