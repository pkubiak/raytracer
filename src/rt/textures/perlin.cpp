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

    for(auto p: octaves){
      float fx = floor(p.second * coord.x), fy = floor(p.second * coord.y), fz = floor(p.second * coord.z);

      v += p.first * lerp3d(
        noise(fx, fy, fz), noise(fx+1.0, fy,fz), noise(fx, fy+1.0,fz),noise(fx+1.0,fy+1.0,fz),
        noise(fx,fy,fz+1.0), noise(fx+1.0, fy, fz+1.0), noise(fx,fy+1.0,fz+1.0), noise(fx+1.0, fy+1.0, fz+1.0),

        p.second*coord.x - fx, p.second*coord.y - fy, p.second * coord.z - fz
      );
    }

    return lerp(black, white, 0.5*(v+1.0f));
  }

  RGBColor PerlinTexture::getColorDX(const Point& coord) {
    NOT_IMPLEMENTED;
  }

  RGBColor PerlinTexture::getColorDY(const Point& coord) {
    NOT_IMPLEMENTED;
  }



}
