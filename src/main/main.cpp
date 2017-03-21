
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/kdtree.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/movingperspective.h>
#include <rt/integrators/casting.h>
#include <ctime>
#include <chrono>
#include <thread>
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using namespace rt;

int main() {
    Image img(800, 600);

    auto start = system_clock::now();
    KDTree* scene = new KDTree();

    // scene->add(new Sphere(Point(2.5f,  .5f,  -1), 0.5  , nullptr, nullptr));
    // scene->add(new Sphere(Point(2.5f,  -1.f,  -1), 0.5, nullptr, nullptr));
    // scene->add(new Sphere(Point(4.5f,  .5f,  -1), 0.5 , nullptr, nullptr));

    for(int x = -5;x<=5;x+=2)
      for(int z=-5;z<=5;z+=2)
        loadOBJ(scene, "models/", "dude.obj", nullptr, Vector(x,0,z));

    auto stop = system_clock::now();
    printf("loadScene time: %f\n", (float)duration_cast<nanoseconds>(stop - start).count()/1000000000.0);
    auto start2 = system_clock::now();

  	scene->rebuildIndex();
    auto stop2 = system_clock::now();
    printf("rebuildIndex time: %f\n", (float)duration_cast<nanoseconds>(stop2 - start2).count()/1000000000.0);

    printf("Primitives count: %d\n", scene->primitives.size());
    World world;
    world.scene = scene;

    PerspectiveCamera cam1(2.0*Point(-5.85f, 7.85f, 7.0f), Vector(1.0f,-1.0f,-1.0f), Vector(0, 1, 0), pi/8, pi/6);
    // PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);

    RayCastingIntegrator integrator(&world);

    // Renderer engine1(&cam1, &integrator);
    // auto start3 = system_clock::now();
    // engine1.render(img);
    // auto stop3 = system_clock::now();
    // printf("render time: %f\n", (float)duration_cast<nanoseconds>(stop3 - start3).count()/1000000000.0);
    // img.writePNG("B1.png");

    // Moving camera
    MovingPerspectiveCamera cam3(
      [](float t)->Point{return Point(14*cos(t), 14.0, 14*sin(t));},
      [](float t)->Point{return Point(-4.0*cos(t), 0.0, -4.0*sin(t));},
      [](float t)->Vector{return Vector(0,1,0);},
      pi/8, pi/6);

    Renderer engine3(&cam3, &integrator);
    char filename[20];
    for(float t=0.0; t<6.28; t+= 0.01){
      printf("Rendering: %f\n", t);
      cam3.setTime(t);
      engine3.render(img);
      sprintf(filename, "./cam3_%.3f.png", t);
      img.writePNG(filename);
    }

    // Renderer engine2(&cam2, &integrator);
    // engine2.render(img);
    // img.writePNG("a4-2.png");
    return 0;
}
