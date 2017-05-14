#include "gradient.h"
#include <core/assert.h>
#include <vector>
#include <core/interpolate.h>
#include <rt/textures/constant.h>
#include <cmath>

using namespace std;

namespace rt {
  GradientTexture::GradientTexture(vector<pair<float, RGBColor> > _colors, Texture* _base, ChannelMixer _mixer):
    textures(), base(_base), mixer(_mixer){
      for(auto p:_colors)
        textures.push_back(make_pair(p.first, new ConstantTexture(p.second)));
    }

  GradientTexture::GradientTexture(vector<pair<float, Texture*> > _textures, Texture* _base, ChannelMixer _mixer):
    textures(_textures), base(_base), mixer(_mixer){}

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
    if(t < (*textures.begin()).first)
      return (*textures.begin()).second->getColor(coord);
    if(t >= (*textures.rbegin()).first)
      return (*textures.rbegin()).second->getColor(coord);

    for(int i=0;i<textures.size()-1;i++)
      if(textures[i].first <= t && t < textures[i+1].first){
        t = (t-textures[i].first)/(textures[i+1].first-textures[i].first);
        t = 1.0f/(1.0f + exp(-3*(t-0.5f)));
        // printf("%f %f\n", t,t2);

        return lerp(textures[i].second->getColor(coord), textures[i+1].second->getColor(coord), t);
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
