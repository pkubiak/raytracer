#include "gradient.h"
#include <core/assert.h>
#include <vector>
#include <core/interpolate.h>

using namespace std;

namespace rt {
  GradientTexture::GradientTexture(vector<pair<float, RGBColor> > _colors, Texture* _base, ChannelMixer _mixer):
    colors(_colors), base(_base), mixer(_mixer){}

  RGBColor GradientTexture::getColor(const Point& coord){
    float t = 0.0;
    RGBColor color = base->getColor(coord);

    switch(mixer){
      case RED:
        t = color.r;break;
      case GREEN:
        t = color.g;break;
      case BLUE:
        t = color.b;break;
      case AVERAGE:
        t = (color.r+color.g+color.b)/3.0;break;
    }
    if(t < (*colors.begin()).first)
      return (*colors.begin()).second;
    if(t > (*colors.rbegin()).first)
      return (*colors.rbegin()).second;

    for(int i=0;i<colors.size()-1;i++)
      if(colors[i].first <= t && t < colors[i+1].first){
        t = (t-colors[i].first)/(colors[i+1].first-colors[i].first);

        return lerp(colors[i].second, colors[i+1].second, t);
      }
    UNREACHABLE;
  }

  RGBColor GradientTexture::getColorDX(const Point& coord){
    NOT_IMPLEMENTED;
  }

  RGBColor GradientTexture::getColorDY(const Point& coord){
    NOT_IMPLEMENTED;
  }

}
