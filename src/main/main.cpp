
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
#include <rt/materials/lambertian.h>
#include <rt/materials/mirror.h>
#include <rt/lights/arealight.h>

#include <rt/lights/pointlight.h>

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


void a12renderCornellbox(float scale, const char* filename, Camera* cam, int numSamples=1) {
    Image img(400, 400);
    World world;
    SimpleGroup* scene = new SimpleGroup();
    world.scene = scene;

    Texture* redtex = new ConstantTexture(RGBColor(.7f,0.f,0.f));
    Texture* greentex = new ConstantTexture(RGBColor(0.f,.7f,0.f));
    Texture* yellowtex = new ConstantTexture(RGBColor(0.7f,.7f,0.f));
    Texture* bluetex = new ConstantTexture(RGBColor(0.f,0.f,0.7f));
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

    Material* grey = new LambertianMaterial(blacktex, whitetex);
    Material* yellowmat = new LambertianMaterial(blacktex, yellowtex);
    Material* leftWallMaterial = new LambertianMaterial(blacktex, redtex);
    Material* rightWallMaterial = new LambertianMaterial(blacktex, greentex);
    Material* blue = new LambertianMaterial(blacktex, bluetex);

    Material* mirror = new MirrorMaterial(2.485, 3.433);
    

    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, blue)); //floor
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(-550.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, nullptr, grey)); //ceiling
    scene->add(new Quad(Point(000.f,000.f,560.f)*scale, Vector(550.f,000.f,000.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, grey)); //back wall
    scene->add(new Quad(Point(000.f,000.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,550.f,000.f)*scale, nullptr, rightWallMaterial)); //right wall
    scene->add(new Quad(Point(550.f,550.f,000.f)*scale, Vector(000.f,000.f,560.f)*scale, Vector(000.f,-550.f,000.f)*scale, nullptr, leftWallMaterial)); //left wall

    scene->add(new Sphere(Point(150.0f, 100.0f, 240.0f)*scale, 99.0f*scale, nullptr, mirror));
    scene->add(new Sphere(Point(450.0f, 50.0f, 50.0f)*scale, 49.0f*scale, nullptr, yellowmat));

    //tall box
    makeBox(scene, Point(265.f, 000.1f, 296.f)*scale, Vector(158.f, 000.f, -049.f)*scale, Vector(049.f, 000.f, 160.f)*scale, Vector(000.f, 330.f, 000.f)*scale, nullptr, grey);

    //Lights
    ConstantTexture* lightsrctex = new ConstantTexture(RGBColor::rep(25.0f));
    Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);

    Quad* light = new Quad(Point(213*scale,549.99f*scale,227*scale), Vector(130*scale,0,0), Vector(0,0,105*scale), nullptr, lightsource);
    AreaLight als(light);
    world.light.push_back(&als);
    scene->add(light);

    //point light
    world.light.push_back(new PointLight(Point(490*scale,159.99f*scale,279.5f*scale),RGBColor(40000.0f*scale*scale,0,0)));
    world.light.push_back(new PointLight(Point(40*scale,159.99f*scale,249.5f*scale),RGBColor(5000.0f*scale*scale,30000.0f*scale*scale,5000.0f*scale*scale)));

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

    a12renderCornellbox(0.001f, "a12-1.png", cam, 1);
    a12renderCornellbox(0.001f, "a12-2.png", cam, 30);
    a12renderCornellbox(0.001f, "a12-3.png", cam, 100);
    return 0;
}
