#ifndef CG1RAYTRACER_RENDERER_HEADER
#define CG1RAYTRACER_RENDERER_HEADER

#include <core/scalar.h>

namespace rt {

class Image;
class Camera;
class Integrator;
class RGBColor;

class Renderer {
public:
    Renderer(Camera* cam, Integrator* integrator);
    void setSamples(uint samples);
    void render(Image& img);
    void renderFlat(Image& img, RGBColor (*func)(uint,uint,uint,uint));
};

}

#endif
