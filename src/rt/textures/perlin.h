#ifndef CG1RAYTRACER_TEXTURES_PERLIN_HEADER
#define CG1RAYTRACER_TEXTURES_PERLIN_HEADER

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/scalar.h>

#include <vector>

namespace rt {

    class PerlinTexture : public Texture {
    protected:
      std::vector<std::pair<float, float> > octaves;
      RGBColor white, black;
    public:
        PerlinTexture(const RGBColor& white, const RGBColor& black);
        void addOctave(float amplitude, float frequency);
        virtual RGBColor getColor(const Point& coord);
        virtual RGBColor getColorDX(const Point& coord);
        virtual RGBColor getColorDY(const Point& coord);
    };

}

#endif
