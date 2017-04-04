#ifndef CG1RAYTRACER_TEXTURES_TEXTURE_HEADER
#define CG1RAYTRACER_TEXTURES_TEXTURE_HEADER

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

#endif
