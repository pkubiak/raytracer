#include "imagetex.h"
#include <core/assert.h>
#include <core/point.h>
#include <core/interpolate.h>

namespace rt {

  ImageTexture::ImageTexture() {}
  ImageTexture::ImageTexture(const Image& _image, BorderHandlingType _bh, InterpolationType _it):
    image(_image), bh(_bh), it(_it){

  }

  ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType _bh, InterpolationType _it):
    image(), bh(_bh), it(_it) {
    image.readPNG(filename);
  }

  inline void clamp(float& v, float min, float max){
    v = (v < min ? min : (v > max ? max : v));
  }

  RGBColor ImageTexture::getColor(int x, int y){//x >= 0, y>=0
    int h = image.height(), w = image.width();

    if(bh == CLAMP){
      x = (x < 0 ? 0 : (x >= w ? w-1 : x));
      y = (y < 0 ? 0 : (y >= h ? h-1 : y));
    }else
    if(bh == MIRROR){
      x = ((x%(2*w))+2*w)%(2*w);
      if(x >= w)
        x = 2*w-1-x;

      y = ((y%(2*h))+2*h)%(2*h);
      if(y >= h)
        y = 2*h - 1 - y;
    }else
    if(bh == REPEAT){
      x = ((x%w)+w)%w;
      y = ((y%h)+h)%h;
    }else
      UNREACHABLE;

    // printf("%d, %d\n",x,y);
    return image(x, y);
  }

  RGBColor ImageTexture::getColor(const Point& coord) {
    float x = coord.x, y = coord.y;

    // TODO: Why not -0.5?
    x = x*image.width();
    y = y*image.height();

    int px = floor(x), py = floor(y);

    float scalex = x-px, scaley = y-py;

    if(it == NEAREST){
      scalex = round(scalex);
      scaley = round(scaley);
    }


    return lerp2d(getColor(px, py), getColor(px+1, py), getColor(px, py+1), getColor(px+1, py+1),
      scalex, scaley
    );
  }

  RGBColor ImageTexture::getColorDX(const Point& coord) {
    if(it == NEAREST){
      float x = coord.x * image.width();
      float y = coord.y * image.height();

      int px = floor(x);
      int py = round(y-floor(y))+floor(y);

      return image.width()*(getColor(px+1, py) - getColor(px, py));
    }else{
      float x = coord.x, y = coord.y;

      x = x*image.width();
      y = y*image.height();

      int px = floor(x), py = floor(y);

      float scalex = x-px, scaley = y-py;

      return image.width()*lerp2d(getColor(px+1, py)-getColor(px-1,py), getColor(px+2, py)-getColor(px,py), getColor(px+1, py+1)-getColor(px-1,py+1), getColor(px+2, py+1)-getColor(px, py+1),
        scalex, scaley
      );
    }

    UNREACHABLE;
  }

  RGBColor ImageTexture::getColorDY(const Point& coord) {
    if(it == NEAREST){
      float x = coord.x * image.width();
      float y = coord.y * image.height();

      int px = round(x-floor(x))+floor(x);
      int py = floor(y);

      return image.height()*(getColor(px, py+1) - getColor(px, py));
    }else{
      float x = coord.x, y = coord.y;

      x = x*image.width();
      y = y*image.height();

      int px = floor(x), py = floor(y);

      float scalex = x-px, scaley = y-py;

      return image.height()*lerp2d(getColor(px, py+1)-getColor(px,py-1), getColor(px+1, py+1)-getColor(px+1,py-1), getColor(px, py+2)-getColor(px,py), getColor(px+1, py+2)-getColor(px+1,py),
        scalex, scaley
      );

    }
    UNREACHABLE;
  }
}
