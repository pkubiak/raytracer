#include "renderer.h"
#include "../core/image.h"
#include "../core/assert.h"
#include "ray.h"
#include "integrators/integrator.h"

namespace rt {
  Renderer::Renderer(Camera* cam, Integrator* integrator): camera(cam), integrator(integrator) {
    //printf("fx: %f %f %f")
  }

  void Renderer::render(Image& img) {
    auto width = img.width();
    auto height = img.height();

    assert(width >= 2 && height >=2);

    #pragma omp parallel for
    for(auto y = 0; y < height; y++){
      for(auto x = 0; x < width; x++){
        // if(x!=700||y!=400)continue;
        // printf("[%d, %d]\n", x,y);
        float px = 2.0*(float)x/(width-1) - 1.0, py = -(2.0*(float)y/(height-1) - 1.0);
        Ray ray = camera->getPrimaryRay(px, py);
        img(x,y) = integrator->getRadiance(ray);
        // printf("\n");
      }
      // return ;
      // printf("%d\n", y);
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
