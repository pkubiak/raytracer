#include "renderer.h"
#include "../core/image.h"
#include "../core/assert.h"
#include "ray.h"

namespace rt {
  Renderer::Renderer(Camera* cam, Integrator* integrator): camera(cam) {
    //printf("fx: %f %f %f")
  }

  void Renderer::render(Image& img) {
    auto width = img.width();
    auto height = img.height();

    assert(width >= 2 && height >=2);

    #pragma omp parallel for
    for(auto y = 0; y < height; y++)
      for(auto x = 0; x < width; x++){
        float px = 2.0*(float)x/(width-1) - 1.0, py = -(2.0*(float)y/(height-1) - 1.0);
        Ray ray = camera->getPrimaryRay(px, py);
        Vector& dir = ray.direction;
        img(x, y) = (RGBColor(dir.x, dir.y, dir.z).clamp());
        //printf("%d %d, %f %f\n", x, y, px, py);
        //printf("%f %f %f %f\n", dir.x, dir.y, dir.z, dir.length());
      }
  }

  void Renderer::renderFlat(Image& img, RGBColor (*func)(uint,uint,uint,uint)) {
    auto width = img.width();
    auto height = img.height();

    #pragma omp parallel for
    for(auto y = 0; y < height; y++)
      for(auto x = 0; x < width; x++)
        img(x, y) = func(x, y, width, height);
  }

}
