#include <rt/textures/perlin.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <algorithm>

namespace rt {
  /* returns a value in range -1 to 1 */
  float noise(int x, int y, int z) {
      int n = x + y * 57 + z * 997;
      n = (n<<13) ^ n;
      return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
  }

  PerlinTexture::PerlinTexture(const RGBColor& _white, const RGBColor& _black): octaves(), white(_white), black(_black) {}

  void PerlinTexture::addOctave(float amplitude, float frequency) {
    octaves.push_back(std::make_pair(amplitude, frequency));
  }

  RGBColor PerlinTexture::getColor(const Point& coord) {
    float v = 0.0;

    for(auto p: octaves)
      v += p.first * noise(p.second*coord.x, p.second * coord.y, p.second * coord.z);

    if(v < -1.0)v = -1.0;
    if(v > 1.0)v = 1.0;

    return lerp(black, white, 0.5*(v+1.0));
  }

  RGBColor PerlinTexture::getColorDX(const Point& coord) {
    NOT_IMPLEMENTED;
  }

  RGBColor PerlinTexture::getColorDY(const Point& coord) {
    NOT_IMPLEMENTED;
  }



}
