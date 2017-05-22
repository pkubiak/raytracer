
#include <core/image.h>
#include <core/color.h>

#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/cameras/perspective.h>
#include <rt/textures/constant.h>
#include <rt/renderer.h>
#include <rt/integrators/recraytrace.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/materials/flatmaterial.h>


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


void a9renderCornellbox(float scale, const char* filename, Camera* cam, int numSamples) {
    Image img(100, 100);
    World world;
    SimpleGroup* scene = new SimpleGroup();
    world.scene = scene;

    Texture* redtex = new ConstantTexture(RGBColor(.7f,0.f,0.f));
    Texture* greentex = new ConstantTexture(RGBColor(0.f,.7f,0.f));
    Texture* yellowtex = new ConstantTexture(RGBColor(0.7f,.7f,0.f));
    Texture* bluetex = new ConstantTexture(RGBColor(0.f,0.f,0.7f));
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* darktex = new ConstantTexture(RGBColor::rep(0.1f));
    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

    Material* grey = new FlatMaterial(whitetex);
    Material* yellow = new FlatMaterial(yellowtex);
    Material* red = new FlatMaterial(redtex);
    Material* green = new FlatMaterial(greentex);
    Material* blue = new FlatMaterial(bluetex);
    Material* dark = new FlatMaterial(darktex);
    Material* black = new FlatMaterial(blacktex);
    
    
    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, yellow)); //floor
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(-550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, grey)); //ceiling
    scene->add(new Quad(Point(000.f,000.f,560.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, dark)); //back wall
    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, green)); //right wall
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,-550.f,000.f)*scale, nullptr, red)); //left wall

    scene->add(new Sphere(Point(150.0f, 100.0f, 240.0f)*scale, 99.0f*scale, nullptr, blue));
    scene->add(new Sphere(Point(450.0f, 50.0f, 50.0f)*scale, 49.0f*scale, nullptr, yellow));

    //tall box
    makeBox(scene, Point(265.f, 000.1f, 296.f)*scale, Vector(158.f, 000.f, -049.f)*scale, Vector(049.f, 000.f, 160.f)*scale, Vector(000.f, 330.f, 000.f)*scale, nullptr, grey);

    RecursiveRayTracingIntegrator integrator(&world);

    Renderer engine(cam, &integrator);
    if (numSamples>1)
        engine.setSamples(numSamples);
    engine.render(img);
    img.writePNG(filename);
}

}

int main() {
    PerspectiveCamera* cam = new PerspectiveCamera(Point(0.278f, 0.273f, -0.800f), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);

    a9renderCornellbox(0.001f, "a11-1.png", cam, 1);
    a9renderCornellbox(0.001f, "a11-2.png", cam, 30);
    return 0;
}

