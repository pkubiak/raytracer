
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>


#include <rt/cameras/perspective.h>
#include <rt/solids/quad.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/kdtree.h>
#include <rt/materials/dummy.h>
#include <rt/loaders/obj.h>
#include <rt/solids/parallelepiped.h>
#include <rt/solids/mengersponge.h>

#include <rt/lights/pointlight.h>

#include <rt/integrators/raytrace.h>

using namespace rt;

namespace {
void makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
    scene->add(new Quad(aaa, forward, left, texMapper, material));
    scene->add(new Quad(aaa, forward, up, texMapper, material));
    scene->add(new Quad(aaa, left, up, texMapper, material));
    Point bbb = aaa + forward + left + up;
    scene->add(new Quad(bbb, -forward, -left, texMapper, material));
    scene->add(new Quad(bbb, -forward, -up, texMapper, material));
    scene->add(new Quad(bbb, -left, -up, texMapper, material));
}

void renderCornellboxA(float scale, const char* filename) {
    Image img(300, 300);
    World world;
    KDTree* scene = new KDTree();
    world.scene = scene;

    PerspectiveCamera cam(Point(278*scale, 273*scale, -800*scale), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);

    DummyMaterial* mat = new DummyMaterial();
    // TODO: Coś się psuje przy ładowaniu obj
    // loadOBJ(scene, "models/", "armadillo.obj", nullptr, Vector(2.4, 1.0, 1.2));
    // TODO: Co należy robić gdy element nie ma przypisanego materiału?

    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, mat)); //floor
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(-550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, mat)); //ceiling
    scene->add(new Quad(Point(000.f,000.f,560.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, mat)); //back wall
    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, mat)); //right wall
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,-550.f,000.f)*scale, nullptr, mat)); //left wall

    //short box
    // makeBox(scene, Point(082.f, 000.1f, 225.f)*scale, Vector(158.f, 000.f, 047.f)*scale, Vector(048.f, 000.f, -160.f)*scale, Vector(000.f, 165.f, 000.f)*scale, nullptr, mat);

    //tall box
    // makeBox(scene, Point(265.f, 000.1f, 296.f)*scale, Vector(158.f, 000.f, -049.f)*scale, Vector(049.f, 000.f, 160.f)*scale, Vector(000.f, 330.f, 000.f)*scale, nullptr, mat);
    // scene->add(new Triangle(Point(0,0,0)*scale, Point(550,0,500)*scale, Point(0,0,560)*scale, nullptr, mat));

    scene->add(new MengerSponge(Point(265.f, 000.1f, 296.f)*scale, Vector(158.f, 000.f, -049.f)*scale, Vector(049.f, 000.f, 160.f)*scale, Vector(000.f, 330.f, 000.f)*scale, 5, nullptr, mat));

    scene->add(new MengerSponge(Point(082.f, 000.1f, 225.f)*scale, Vector(158.f, 000.f, 047.f)*scale, Vector(048.f, 000.f, -160.f)*scale, Vector(000.f, 165.f, 000.f)*scale, 5, nullptr, mat));
    scene->setMaterial(mat);

    scene->rebuildIndex();

    //point light
    Point p = Point(082.f, 000.1f, 225.f)*scale + Vector(158.f, 000.f, 047.f)*scale*0.5 + Vector(048.f, 000.f, -160.f)*scale*0.5 + Vector(000.f, 165.f, 000.f)*scale*0.5;
    world.light.push_back(new PointLight(p,RGBColor::rep(4000.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(288*scale,529.99f*scale,279.5f*scale),RGBColor::rep(40000.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(490*scale,329.99f*scale,279.5f*scale),RGBColor(60000.0f*scale*scale,0,0)));
    world.light.push_back(new PointLight(Point(40*scale,329.99f*scale,279.5f*scale),RGBColor(0,60000.0f*scale*scale,0)));
    world.light.push_back(new PointLight(Point(220*scale,129.99f*scale,79.5f*scale),RGBColor(0,0,3*6000.0f*scale*scale)));

    RayTracingIntegrator integrator(&world);

    Renderer engine(&cam, &integrator);
    engine.render(img);
    img.writePNG(filename);
}

}

int main(int argc, char* argv[]) {
    renderCornellboxA(0.001f, "a4-1.png");
    renderCornellboxA(0.01f, "a4-2.png");
    return 0;
}
