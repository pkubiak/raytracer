
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
#include <rt/integrators/casting.h>

using namespace rt;

int main() {
    Image img(800, 600);

    KDTree* scene = new KDTree();

    scene->add(new Sphere(Point(2.5f,  .5f,  -1), 0.5  , nullptr, nullptr));
    scene->add(new Sphere(Point(2.5f,  -1.f,  -1), 0.5, nullptr, nullptr));
    scene->add(new Sphere(Point(4.5f,  .5f,  -1), 0.5 , nullptr, nullptr));

    loadOBJ(scene, "models/", "dude.obj");

  	scene->rebuildIndex();
    // printf("Sizeof: %d\n", sizeof(KDTreeNode));
    // printf("nodes:%d leaves:%d primitives:%d\n", scene->nodes.size(), scene->leaves.size(),scene->primitives.size());
    // int suma = 0;
    // for(auto x: scene->leaves){
    //   // printf(">%d\n",x->size());
    //   suma+=x->size();
    // }
    // printf("SUMA: %d\n", suma);
    World world;
    world.scene = scene;

    PerspectiveCamera cam1(Point(-5.85f, 7.85f, 7.0f), Vector(1.0f,-1.0f,-1.0f), Vector(0, 1, 0), pi/8, pi/6);
    // PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);
    RayCastingIntegrator integrator(&world);

    Renderer engine1(&cam1, &integrator);
    engine1.render(img);
    img.writePNG("a4-1.png");
    //
    // Renderer engine2(&cam2, &integrator);
    // engine2.render(img);
    // img.writePNG("a4-2.png");
    return 0;
}
