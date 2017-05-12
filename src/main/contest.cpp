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


void main_contest(){
  Camera* cam = new PerspectiveCamera(Point(0.048f, 1.143f, -0.050f), Vector(-0.4f, -0.4f, 0.7f), Vector(0, 1, 0), 0.686f, 0.686f);
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
    {0.3, new ImageTexture("textures/grass2.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    {0.4, new ImageTexture("textures/grass3.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    {0.5, new ImageTexture("textures/sand.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)},
    {0.6, new ImageTexture("textures/water.png", ImageTexture::REPEAT, ImageTexture::BILINEAR)}
  }, &mask, GradientTexture::RED);


  FlatMaterial material(&texture);

  scene->add(new InfinitePlane(Point(0,0,0),Vector(0,1,0), new PlaneCoordMapper(Vector(1,0,0), Vector(0,0,1)), &material));

  RayTracingIntegrator integrator(&world);

  Renderer engine(cam, &integrator);
  engine.render(img);
  // engine.setSamples(9);
  img.writePNG("fantasy.png");
}
