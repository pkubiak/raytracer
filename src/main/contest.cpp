#include <core/image.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/infiniteplane.h>
#include <rt/integrators/raytrace.h>
#include <rt/renderer.h>
#include <rt/cameras/perspective.h>
#include <rt/textures/perlin.h>
#include <rt/materials/flatmaterial.h>
#include <rt/coordmappers/plane.h>
#include <rt/textures/constant.h>
#include <rt/textures/gradient.h>
#include <rt/textures/imagetex.h>
#include <core/color.h>

using namespace rt;


void main_gradient_texture(){
  Camera* cam = new PerspectiveCamera(Point(0.048f, 4.143f, -0.050f), Vector(-0.4f, -0.4f, 0.7f), Vector(0, 1, 0), 0.686f, 0.686f);
  Image img(1800, 1800);
  World world;
  SimpleGroup* scene = new SimpleGroup();
  world.scene = scene;

  PerlinTexture mask(RGBColor(0,0,0), RGBColor(1,1,1));
  mask.addOctave(0.5, 1);
  mask.addOctave(0.25, 2);
  mask.addOctave(0.125, 8);
  // mask.addOctave(0.05, 128);

  GradientTexture texture({
    {0.3, new ImageTexture("textures/grass6.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    // {0.42, new ImageTexture("textures/grass5.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    {0.56, new ImageTexture("textures/grass7.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    {0.7, new ImageTexture("textures/grass8.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)}
  }, &mask, GradientTexture::RED);


  FlatMaterial material(&texture);

  scene->add(new InfinitePlane(Point(0,0,0),Vector(0,1,0), new PlaneCoordMapper(Vector(1,0,0), Vector(0,0,1)), &material));

  RayTracingIntegrator integrator(&world);

  Renderer engine(cam, &integrator);
  engine.render(img);

  img.writePNG("fantasy.png");
}
