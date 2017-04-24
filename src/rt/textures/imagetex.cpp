#include "imagetex.h"
#include <core/assert.h>
#include <core/point.h>

namespace rt {

  ImageTexture::ImageTexture() {}
  ImageTexture::ImageTexture(const Image& _image, BorderHandlingType _bh, InterpolationType _it):
    image(_image), bh(_bh), it(_it){

  }

  ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType _bh, InterpolationType _it):
    image(), bh(_bh), it(_it) {
    image.readPNG(filename);
  }

  RGBColor ImageTexture::getColor(const Point& coord) {
    float x = coord.x, y = coord.y;
    if(bh == CLAMP){
      if(x < 0)x = 0;
      if(x > 1)x = 1;
      if(y < 0)y = 0;
      if(y > 1)y = 1;
    }else
    if(bh == MIRROR){
      x = fmod(x, 2.0f);
      if(x >= 1.0f)x = 2.0f - x;
      y = fmod(y, 2.0f);
      if(y >= 1.0f)y = 2.0f - y;
    }else
    if(bh == REPEAT){
      x = fmod(x, 1.0f);
      y = fmod(y, 1.0f);
    }

    float dx = 1.0f/image.width(), dy = 1.0f / image.height();

    if(it == NEAREST){
        return image((int)floor(x / dx), (int)floor(y/dy));
    }else
    if(it == BILINEAR){
      return RGBColor(1.0, 0.5, 0.0);
    }
  }

  RGBColor ImageTexture::getColorDX(const Point& coord) {
    NOT_IMPLEMENTED;
  }

  RGBColor ImageTexture::getColorDY(const Point& coord) {
    NOT_IMPLEMENTED;
  }
}
