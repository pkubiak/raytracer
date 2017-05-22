#include "renderer.h"
#include "../core/image.h"
#include "../core/assert.h"
#include "ray.h"
#include "integrators/integrator.h"
#include <core/random.h>

namespace rt {
  Renderer::Renderer(Camera* cam, Integrator* integrator): samples(1), camera(cam), integrator(integrator) {}

  void Renderer::render(Image& img) {
    auto width = img.width();
    auto height = img.height();

    assert(width >= 2 && height >=2);

    if(samples == 1){
      #pragma omp parallel for schedule(dynamic, 10)
      for(auto y = 0; y < height; y++){
        for(auto x = 0; x < width; x++){
          float px = -1.0 + 2.0*(x+0.5)/width, py = 1.0 - 2.0*(y+0.5)/height;

          Ray ray = camera->getPrimaryRay(px, py);
          img(x,y) = integrator->getRadiance(ray);
        }
      }
    }else{
      #pragma omp parallel for schedule(dynamic, 10)
      for(auto y = 0; y < height; y++)
        for(auto x = 0; x < width; x++){
          RGBColor c(0,0,0);

          for(int s = 0; s < samples; s++){
            float px = -1.0 + 2.0*(x+0.5+rt::random(-0.5,0.5))/width, py = 1.0 - 2.0*(y+0.5+rt::random(-0.5, 0.5))/height;

            Ray ray = camera->getPrimaryRay(px, py);
            c = c + integrator->getRadiance(ray);
          }

          img(x,y) = c/(float)samples;
        }
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

  void Renderer::setSamples(uint samples_){
    samples = samples_;
  }
}
