
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

float ranf(){
  return (float)(rand()%10000)/5000.0-1.0;
}
int main() {
    Image img(1920, 1280);
    // Image img(640, 480);

    auto start = system_clock::now();
    KDTree* scene = new KDTree();

    // for(int i=0;i<2000;i++){
    //   scene->add(new Sphere(Point(7.0*ranf(),  7.0*ranf(),  10.0*ranf()+16.0), 0.9  , nullptr, nullptr));
    // }
    // scene->add(new Sphere(Point(2.5f,  -1.f, 7), 0.5, nullptr, nullptr));
    // scene->add(new Sphere(Point(4.5f,  .5f,  12), 0.5 , nullptr, nullptr));

    // for(int x = -3;x<=0;x++)
    //  for(int z=-3;z<=0;z++)
        // loadOBJ(scene, "models/", "dude.obj", nullptr, Vector(0,0,0));
      // loadOBJ(scene, "models/", "armadillo.obj", nullptr, Vector(0,0,0));
      // for(int i=0;i<1;i++)
    // Vector v(0,1,2);
    // printf("%p %p %p\n", &v.x, &v.y, &v.z);
    // printf("%f %f %f\n", v(0), v(1), v(2));

    for(int x = -4;x<=4;x++)
      for(int z=-4;z<=4;z++)
        loadOBJ(scene, "models/", "dude.obj", nullptr, Vector(2*x,0,2*z));

    auto stop = system_clock::now();
    printf("loadScene time: %f\n", (float)duration_cast<nanoseconds>(stop - start).count()/1000000000.0);
    auto start2 = system_clock::now();

  	scene->rebuildIndex();
    auto stop2 = system_clock::now();
    printf("rebuildIndex time: %f\n", (float)duration_cast<nanoseconds>(stop2 - start2).count()/1000000000.0);

    printf("Primitives count: %d\n", scene->primitives.size());
    World world;
    world.scene = scene;

    // PerspectiveCamera cam1(2.0*Point(0.0f, 0.25f, -8.0f), Vector(0.0f,0.0f, 1.0f), Vector(0, 1, 0), pi/8, pi/6);
    // PerspectiveCamera cam1(2.0*Point(0.0f, 0.95f, -5.0f), Vector(0.0f,0.0f, 1.0f), Vector(0, 1, 0), pi/8, pi/6);
    PerspectiveCamera cam1(2.0*Point(-5.85f, 7.85f, 7.0f), Vector(1.0f,-1.0f,-1.0f), Vector(0, 1, 0), pi/8, pi/6);
    // PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);

    RayCastingIntegrator integrator(&world);

    Renderer engine1(&cam1, &integrator);
    auto start3 = system_clock::now();
    engine1.render(img);
    auto stop3 = system_clock::now();
    printf("render time: %f\n", (float)duration_cast<nanoseconds>(stop3 - start3).count()/1000000000.0);
    img.writePNG("B1.png");
    delete scene;
    // Moving camera
    // MovingPerspectiveCamera cam3(
    //   [](float t)->Point{return Point(18*cos(t), 15.0+5.0*sin(t), 18*sin(t));},
    //   [](float t)->Point{return Point(4.0*cos(3.0*t), 0.0, 4.0*sin(3.0*t));},
    //   [](float t)->Vector{return Vector(0,1,0);},
    //   pi/8, pi/6);
    //
    // Renderer engine3(&cam3, &integrator);
    // char filename[20];
    // int frames = (int)(6.28/0.005);
    //
    // auto start3 = system_clock::now();
    // for(int i=0;i<frames;i++){
    //   float t = 6.28*i/frames;
    //   printf("Rendering: %f\n", t);
    //   cam3.setTime(t);
    //   engine3.render(img);
    //   sprintf(filename, "./cam3_%.4d.png", i);
    //   img.writePNG(filename);
    // }
    // auto stop3 = system_clock::now();
    // printf("render time: %f\n", (float)duration_cast<nanoseconds>(stop3 - start3).count()/1000000000.0);
    // printf("render time per frame: %f\n", (float)duration_cast<nanoseconds>(stop3 - start3).count()/1000000000.0/frames);

    // Renderer engine2(&cam2, &integrator);
    // engine2.render(img);
    // img.writePNG("a4-2.png");
    return 0;
}
