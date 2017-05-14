#include <core/image.h>
#include <core/color.h>

#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/kdtree.h>
#include <rt/loaders/obj.h>

#include <rt/integrators/recraytrace.h>
#include <rt/renderer.h>
#include <rt/cameras/perspective.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/sphere.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/cooktorrance.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/mirror.h>
#include <rt/materials/phong.h>


#include <rt/coordmappers/plane.h>

#include <rt/textures/perlin.h>
#include <rt/textures/constant.h>
#include <rt/textures/gradient.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/checkerboard.h>

#include <rt/lights/directional.h>
#include <rt/lights/pointlight.h>

using namespace rt;


void main_cooktorrance(){
  Camera* cam = new PerspectiveCamera(Point(0.048f, 4.143f, -0.050f), Vector(-0.4f, -0.4f, 0.7f), Vector(0, 1, 0), 0.686f, 0.686f);
  Image img(1800, 1800);
  World world;
  SimpleGroup* scene = new SimpleGroup();
  world.scene = scene;

  ConstantTexture black(RGBColor(0,0,0));
  ConstantTexture white(RGBColor(1,1,1));

  LambertianMaterial checkboard(
    &black,
    new CheckerboardTexture(RGBColor(1,1,1), RGBColor(0,0,0))
  );



  // FlatMaterial material(&texture);
  CookTorranceMaterial cooktorrance(&white, 0.177, 3.638, 10, 1000);
  LambertianMaterial lambertian(&black, &white);
  PhongMaterial phong(&white, 0.1f);

  MirrorMaterial mirror(0.177, 3.638);

  Vector d = cross(Vector(-0.4f, -0.4f, 0.7f), Vector(0,1,0));

  // SimpleGroup* scene1 = new SimpleGroup();
  // loadOBJ(scene1, "models/", "dude.obj", nullptr, Vector(-4,1,4));
  // scene1->setMaterial(&cooktorrance);
  // scene->add(scene1);

  scene->add(new Sphere(Point(-4,1,4), 0.5f, nullptr, &cooktorrance));
  scene->add(new Sphere(Point(-4,1,4)-1.7*d, 0.5f, nullptr, &lambertian));
  scene->add(new Sphere(Point(-4,1,4)-3.4*d, 0.5f, nullptr, &phong));
  // scene->
  scene->add(new InfinitePlane(Point(0,0,0),Vector(0,1,0), new PlaneCoordMapper(Vector(1,0,0), Vector(0,0,1)), &checkboard));

  // world.light.push_back(new PointLight(Point(0.048f, 7.143f, -0.050f), RGBColor(50,50,50)));
  world.light.push_back(new DirectionalLight(Vector(0, -0.1, 0.7f), RGBColor(5,5,5)));

  RecursiveRayTracingIntegrator integrator(&world);

  Renderer engine(cam, &integrator);
  engine.render(img);

  img.writePNG("cooktorrance.png");
}
