// Paweł Kubiak - pozwalam na opisanie obrazków moim imieniem/nazwiskiem

#include <core/image.h>

#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/perlin.h>
#include <rt/textures/gradient.h>

#include <rt/materials/flatmaterial.h>
#include <rt/solids/sphere.h>
#include <rt/solids/quad.h>
#include <rt/integrators/recraytrace.h>
#include <rt/cameras/perspective.h>
#include <rt/renderer.h>

using namespace rt;

void example_textures(){
  Image img(800, 800);
  World world;
  SimpleGroup* scene = new SimpleGroup();

  PerlinTexture* perlinTex = new PerlinTexture(RGBColor(0.0f,0.0f,0.0f), RGBColor(1.0f,0.0f,0.0f));

  perlinTex->addOctave(0.5f, 5.0f);
  perlinTex->addOctave(0.25f, 10.0f);
  perlinTex->addOctave(0.125f, 20.0f);
  perlinTex->addOctave(0.125f, 40.0f);

  PerlinTexture* perlinTex2 = new PerlinTexture(RGBColor(0.0f,0.0f,0.0f), RGBColor(1.0f,0.0f,0.0f));

  perlinTex2->addOctave(0.5f, 10.0f);
  perlinTex2->addOctave(0.25f, 30.0f);
  perlinTex2->addOctave(0.125f, 60.0f);
  perlinTex2->addOctave(0.125f, 80.0f);


  PerlinTexture* perlinTex3 = new PerlinTexture(RGBColor(0.0f,0.0f,0.0f), RGBColor(1.0f,0.0f,0.0f));

  perlinTex3->addOctave(0.5f, 20.0f);
  perlinTex3->addOctave(0.25f, 40.0f);
  perlinTex3->addOctave(0.125f, 80.0f);
  perlinTex3->addOctave(0.125f, 120.0f);


  // Sun
  GradientTexture* sunTex = new GradientTexture({
    {0.25, RGBColor(0,0,0)}, {0.5, RGBColor(0.75,0,0)}, {0.60, RGBColor(1.0, 0.4, 0.15)},
    {0.70, RGBColor(1.0, 1.0, 0.1)}, {0.80, RGBColor(1.0,1.0,1.0)}},
    perlinTex, GradientTexture::RED
  );
  FlatMaterial* sun = new FlatMaterial(sunTex);

  scene->add(new Sphere(Point(0,0,0), 1.0f, nullptr, sun));

  // Mercury
  GradientTexture* mercuryTex = new GradientTexture(
    {{0.0, RGBColor(0.1,0.1,0.1)}, {1.0, RGBColor(0.9,0.9,0.9)}},
    perlinTex2, GradientTexture::RED
  );
  FlatMaterial* mercury = new FlatMaterial(mercuryTex);

  scene->add(new Sphere(Point(0.5,-1.5,0.5), 0.08f, nullptr, mercury));

  // Venus

  GradientTexture* venus = new GradientTexture(
    {
      {0.2, RGBColor(46, 11, 0)/255.0},
      {0.34, RGBColor(106, 38, 0)/255.0},
      {0.5, RGBColor(179, 108, 28)/255.0},
      {0.8, RGBColor(255, 235, 128)/255.0},
    },
    perlinTex2, GradientTexture::RED
  );

  scene->add(new Sphere(Point(1.2,-1.5,0.7), 0.23f, nullptr, new FlatMaterial(venus)));

  // Earth
  GradientTexture* earth = new GradientTexture(
    {
      {0.3, RGBColor(31, 113, 198)/255.0},
      {0.50, RGBColor(69, 168, 226)/255.0},
      {0.55, RGBColor(61, 117, 108)/255.0},
      {0.65, RGBColor(255, 249, 116)/255.0},
      {0.7, RGBColor(192, 162, 100)/255.0},
      {0.75, RGBColor(133, 93, 68)/255.0},
      {0.9, RGBColor(255, 255, 255)/255.0},

    }, perlinTex2, GradientTexture::RED
  );

  scene->add(new Sphere(Point(0.0,-3,0.9), 0.15f, nullptr, new FlatMaterial(earth)));

  // mars
  GradientTexture* mars = new GradientTexture(
    {
      {0.15, RGBColor(38, 27, 25)/255.0},
      {0.20, RGBColor(79, 49, 39)/255.0},
      {0.32, RGBColor(123, 71, 50)/255.0},
      {0.5, RGBColor(186, 112, 85)/255.0},
      {0.75, RGBColor(193, 148, 151)/255.0},
      {0.85, RGBColor(247, 226, 223)/255.0},
    },
    perlinTex3, GradientTexture::RED
  );
  scene->add(new Sphere(Point(0.8,-3,0.6), 0.28f, nullptr, new FlatMaterial(mars)));

  // StarsBox
  // Textures from: https://opengameart.org/content/ulukais-space-skyboxes under CC-BY 3.0
  ImageTexture* starsTex1 = new ImageTexture("models/cosmos1.png", ImageTexture::CLAMP, ImageTexture::NEAREST);
  ImageTexture* starsTex2 = new ImageTexture("models/cosmos2.png", ImageTexture::CLAMP, ImageTexture::NEAREST);
  ImageTexture* starsTex3 = new ImageTexture("models/cosmos3.png", ImageTexture::CLAMP, ImageTexture::NEAREST);
  ImageTexture* starsTex4 = new ImageTexture("models/cosmos4.png", ImageTexture::CLAMP, ImageTexture::NEAREST);

  float s = 5.0;
  scene->add(new Quad(Point(-s,s,s), Vector(2*s,0,0), Vector(0,0,-2*s), nullptr, new FlatMaterial(starsTex1)));
  scene->add(new Quad(Point(-s,s,-s), Vector(2*s,0,0), Vector(0,-2*s,0), nullptr, new FlatMaterial(starsTex2)));
  scene->add(new Quad(Point(s,s,-s), Vector(0,0,2*s), Vector(0,-2*s,0), nullptr, new FlatMaterial(starsTex3)));
  scene->add(new Quad(Point(-s,s,-s), Vector(0,0,2*s), Vector(0,-2*s,0), nullptr, new FlatMaterial(starsTex4)));

  world.scene = scene;

  PerspectiveCamera cam(3*Point(0.1f, -1.3f, 0.525f), Vector(0, 1, -0.5f), Vector(0, 0, 1), 1.0f, 1.0f);
  RecursiveRayTracingIntegrator integrator(&world);
  Renderer engine(&cam, &integrator);

  engine.render(img);
  img.writePNG("a6-4.png");
}
