
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/solids/infiniteplane.h>
#include <rt/solids/triangle.h>
#include <rt/solids/quad.h>
#include <rt/solids/disc.h>
#include <rt/solids/aabox.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/orthographic.h>
#include <rt/integrators/casting.h>
#include <rt/renderer.h>

using namespace rt;

float randf(float x){
  return x*(2*(float)(rand()%10000)/10000.0-1.0);
}

void a_solids() {
    Image img(640, 480);

    SimpleGroup* scene = new SimpleGroup();

    scene->add(new Sphere(Point(-2.f,  1.7f,  0), 2   , nullptr, nullptr));
    scene->add(new Sphere(Point(1,    -1,     1), 2.2f, nullptr, nullptr));
    scene->add(new Sphere(Point(3.f,   0.8f, -2), 2, nullptr, nullptr));

    scene->add(new InfinitePlane(Point(0,-1, 0), Vector(0, 1, 0), nullptr, nullptr));

    scene->add(new Triangle(Point(-2, 3.7f, 0), Point(1, 2,  1), Point( 3, 2.8f, -2), nullptr, nullptr));
    scene->add(new Triangle(Point(3,  2,    3), Point(3, 2, -3), Point(-3, 2   , -3), nullptr, nullptr));

    scene->add(new Quad(Point(1,  -0.9f,    4.5f), Vector(-2, 0, 0), Vector(0, 0.1f, -2), nullptr, nullptr));
    // scene->add(new Quad(Point(2,  -0.9f,    4.5f), Vector(0, 0.1f, -2), Vector(-2, 0, 0), nullptr, nullptr));

    scene->add(new Disc(Point(-3, -0.75f, 1.5f), Vector(0, 0.5f, 0.5f), 1.5f, nullptr, nullptr));

    scene->add(new AABox(Point(2,1.5f,-0.5f), Point(3,2.5f,2.5f), nullptr, nullptr));

    // for(auto i = 0;i < 10; i++)
      // scene->add(new Quad(Point(randf(6), randf(6), randf(6)), Vector(randf(6), randf(6), randf(6)), Vector(randf(6), randf(6), randf(6)), nullptr, nullptr));
      // scene->add(new Triangle(Point(randf(6), randf(6), randf(6)), Point(randf(6), randf(6), randf(6)), Point(randf(6), randf(6), randf(6)), nullptr, nullptr));
      // scene->add(new Sphere(Point(randf(4),   randf(4), randf(4)), randf(2)+2.0, nullptr, nullptr));
    // Quad q(Point(randf(6), randf(6), randf(6)), Vector(randf(6), randf(6), randf(6)), Vector(randf(6), randf(6), randf(6)), nullptr, nullptr);
    // q.getBounds();

    World world;
    world.scene = scene;

    PerspectiveCamera cam1(Point(0, 0, 10), Vector(0, 0, -1), Vector(0, 1, 0), pi/4, pi/3);
    OrthographicCamera cam2(Point(0, 0.5, 8), Vector(0, -0.1, -1), Vector(0, 1, 0), 14.0f, 11.0f);

    RayCastingIntegrator integrator(&world);
    Renderer engine(&cam1, &integrator);
    engine.render(img);
    img.writePNG("a3-1.png");

    Renderer engine2(&cam2, &integrator);
    engine2.render(img);
    img.writePNG("a3-2.png");
}
