#include <core/image.h>
#include <core/color.h>

#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/kdtree.h>
#include <rt/loaders/obj.h>

#include <rt/integrators/recraytrace.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/casting.h>

#include <rt/renderer.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/fisheye.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/sphere.h>
#include <rt/solids/parallelepiped.h>

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
#include <rt/lights/projective.h>

#include <rt/primmod/instance.h>

using namespace rt;

void demo_projective_light(){
  // Camera* cam = new PerspectiveCamera(Point(0,12,0), Vector(0,-1,0), Vector(1, 0, 0), M_PI/4, M_PI/4);
  Camera* cam = new FishEyeCamera(Point(4,2.5,4), Vector(-1,0.4,-1), Vector(0, 1, 0), 0.65*M_PI);
  // Camera* cam = new FishEyeCamera(Point(0,10,0), Vector(0,-1,0), Vector(1,0, 0), 0.75*M_PI);
  Image img(2400, 2400);
  World world;
  KDTree* scene = new KDTree();
  world.scene = scene;

  ConstantTexture black(RGBColor(0,0,0));
  ConstantTexture white(RGBColor(1,1,1));

  LambertianMaterial checkboard(
    &black,
    new CheckerboardTexture(RGBColor(1,1,1), RGBColor(0,0,0))
  );




  // FlatMaterial material(&texture);
  // CookTorranceMaterial cooktorrance(&white, 0.177, 3.638, 10, 1000);
  LambertianMaterial lambertian(&black, &white);
  // PhongMaterial phong(&white, 0.1f);

  MirrorMaterial mirror(0.177, 3.638);

  // Vector d = cross(Vector(-0.4f, -0.4f, 0.7f), Vector(0,1,0));

  // SimpleGroup* scene1 = new SimpleGroup();
  // loadOBJ(scene1, "models/", "dude.obj", nullptr, Vector(-4,1,4));
  // scene1->setMaterial(&cooktorrance);
  // scene->add(scene1);

  ImageTexture skymap("textures/skymap.png", ImageTexture::CLAMP, ImageTexture::BILINEAR);

  scene->add(new Sphere(Point(0,0,0), 14.0f, nullptr, &lambertian));
  scene->add(new Sphere(Point(0,2,0), 0.6f, nullptr, &lambertian));

  scene->add(new InfinitePlane(Point(0,0.01f,0),Vector(0,1,0), new PlaneCoordMapper(Vector(1,0,0), Vector(0,0,1)), &lambertian));
  // scene->add(new InfinitePlane(Point(0,12.0f,0),Vector(0,-1,0), new PlaneCoordMapper(Vector(1,0,0), Vector(0,0,1)), &lambertian));
  float w = 0.2;
  scene->add(new Parallelepiped(Point(w,0,w), Vector(-2*w,0,0), Vector(0,0,-2*w), Vector(0,2,0), nullptr, &lambertian));
  // scene->add(new Parallelepiped(Point(w,3,w), Vector(-2*w,-2*w,0), Vector(0,-2*w,-2*w), Vector(2,0,2), nullptr, &lambertian));

  KDTree *chair = new KDTree();
  chair->add(new Parallelepiped(Point(-0.5,0,-0.5), Vector(1,0,0), Vector(0,0.5,0), Vector(0,0,1), nullptr, &lambertian));
  chair->add(new Parallelepiped(Point(-0.7,0.5,-0.7), Vector(1.4,0,0), Vector(0,0.2,0), Vector(0,0,1.4), nullptr, &lambertian));
  chair->add(new Parallelepiped(Point(-0.8,0.6,-0.8), Vector(-0.3,1.5,0), 0.3*Vector(1.5,0.3,0), Vector(0,0,1.6), nullptr, &lambertian));

  chair->add(new Parallelepiped(Point(-0.5,0.6,-0.9), 0.3*Vector(-0.3,1.5,0), Vector(1.2,0,0), Vector(0,0,0.2), nullptr, &lambertian));
  chair->add(new Parallelepiped(Point(-0.5,0.6,0.7), 0.3*Vector(-0.3,1.5,0), Vector(1.2,0,0), Vector(0,0,0.2), nullptr, &lambertian));
  chair->rebuildIndex();

  for(int i=0;i<32;i++){
    Instance* c = new Instance(chair);
    float a = 2*M_PI*(float)i/32.0;
    c->scale(0.7);
    c->rotate(Vector(0,1,0), a);
    c->translate(Vector(-10.0*cos(a), 0, 10.0*sin(a)));
    scene->add(c);
  }

  for(int i=0;i<32;i++){
    Instance* c = new Instance(chair);
    float a = 2*M_PI*((float)i+0.5)/32.0;
    c->scale(0.7);
    c->rotate(Vector(0,1,0), a);
    c->translate(Vector(-8.0*cos(a), 0, 8.0*sin(a)));
    scene->add(c);
  }

  for(int i=0;i<24;i++){
    Instance* c = new Instance(chair);
    float a = 2*M_PI*(float)i/24.0;
    c->scale(0.7);
    c->rotate(Vector(0,1,0), a);
    c->translate(Vector(-6.0*cos(a), 0, 6.0*sin(a)));
    scene->add(c);
  }

  // world.light.push_back(new DirectionalLight(Vector(0, 0.1, 0.7f), RGBColor(5,5,5)));
  world.light.push_back(new PointLight(Point(0,5,0),70*RGBColor(0,0,1)));
  world.light.push_back(new ProjectiveLight(Point(0,4,0), Vector(0,1,0), Vector(0,0,1), &skymap, 3.1415*1.2, 500.0));
  // world.light.push_back(new ProjectiveLight(Point(0,8,0), Vector(0,-1,0), Vector(1,0,0), &skymap, 3.1415/3, 500.0));
  scene->rebuildIndex();
  // RayCastingIntegrator integrator(&world);
  RayTracingIntegrator integrator(&world);

  Renderer engine(cam, &integrator);
  engine.render(img);

  img.writePNG("projective-light.png");
}
