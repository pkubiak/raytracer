#pragma once

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>
#include<vector>
using namespace std;

namespace rt {
  class GradientTexture : public Texture {
    public:
      enum ChannelMixer {
        RED, GREEN, BLUE, AVERAGE
      };

      GradientTexture(vector<pair<float, RGBColor> >,Texture*, ChannelMixer);
      virtual RGBColor getColor(const Point& coord);
      virtual RGBColor getColorDX(const Point& coord);
      virtual RGBColor getColorDY(const Point& coord);
    protected:
      vector<pair<float, RGBColor> > colors;
      Texture *base;
      ChannelMixer mixer;
  };
}
