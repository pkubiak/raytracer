#include "renderer.h"
#include "../core/image.h"

namespace rt {
  Renderer::Renderer(Camera* cam, Integrator* integrator) {}

  //void Renderer::render(Image& img) {}

  void Renderer::renderFlat(Image& img, RGBColor (*func)(uint,uint,uint,uint)) {
    auto width = img.width();
    auto height = img.height();

    #pragma omp parallel for
    for(auto y = 0; y < height; y++)
      for(auto x = 0; x < width; x++)
        img(x, y) = func(x, y, width, height);
  }

}
