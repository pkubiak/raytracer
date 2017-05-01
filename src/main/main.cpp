
#include <core/image.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/quad.h>

#include <rt/groups/simplegroup.h>

#include <rt/cameras/perspective.h>

#include <rt/textures/imagetex.h>
#include <rt/materials/flatmaterial.h>
#include <rt/integrators/raytrace.h>


#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/textures/checkerboard.h>
#include <rt/textures/perlin.h>

using namespace rt;

namespace {

    Group* imageTexturesBorder() {

        SimpleGroup* scene = new SimpleGroup();

        ImageTexture* clampTex = new ImageTexture("models/stones_diffuse.png", ImageTexture::CLAMP, ImageTexture::NEAREST);
        FlatMaterial* clamp = new FlatMaterial(clampTex);

        ImageTexture* mirrorTex = new ImageTexture("models/stones_diffuse.png", ImageTexture::MIRROR, ImageTexture::BILINEAR);
        FlatMaterial* mirror = new FlatMaterial(mirrorTex);

        scene->add(new InfinitePlane(Point(0.0f,0.0f,-0.004f), Vector(-0.02f, -0.02f, 1.0f), nullptr, clamp));
        scene->add(new InfinitePlane(Point(0.0f,0.0f,-0.001f), Vector(0.02f, -0.02f, 1.0f), nullptr, mirror));


        return scene;
    }

    Group* imageTexturesInterpolate() {

        SimpleGroup* scene = new SimpleGroup();

        ImageTexture* nearTex = new ImageTexture("models/stones_diffuse.png", ImageTexture::REPEAT, ImageTexture::NEAREST);
        FlatMaterial* near = new FlatMaterial(nearTex);

        ImageTexture* interTex = new ImageTexture("models/stones_diffuse.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
        FlatMaterial* inter = new FlatMaterial(interTex);

        scene->add(new InfinitePlane(Point(0.1f, -1.5f,0.15f), Vector(0.1f, 0.0f, 1.0f), nullptr, near));
        scene->add(new InfinitePlane(Point(0.1f, -1.5f,0.15f), Vector(-0.1f, 0.0f, 1.0f), nullptr, inter));

        return scene;
    }


    Group* proceduralTextures() {
        SimpleGroup* scene = new SimpleGroup();

        CheckerboardTexture* checkerboardTex = new CheckerboardTexture(RGBColor(1.0f,0.9f,0.7f), RGBColor(0.2f,0.2f,0.0f));
        FlatMaterial* checkerboard = new FlatMaterial(checkerboardTex);

        PerlinTexture* perlinTex = new PerlinTexture(RGBColor(1.0f,1.0f,0.9f), RGBColor(0.5f,0.5f,1.0f));
        perlinTex->addOctave(0.5f, 5.0f);
        perlinTex->addOctave(0.25f, 10.0f);
        perlinTex->addOctave(0.125f, 20.0f);
        perlinTex->addOctave(0.125f, 40.0f);
        FlatMaterial* perlin = new FlatMaterial(perlinTex);

        scene->add(new InfinitePlane(Point(0.0f,0.0f,-0.018f), Vector(0.01f, 0.0f, 1.0f), nullptr, checkerboard));
        scene->add(new InfinitePlane(Point(0.0f,0.0f,-0.02f), Vector(-0.01f, 0.0f, 1.0f), nullptr, perlin));

        return scene;
    }

}


int main() {
    Image img(800, 800);
    World world;
    PerspectiveCamera cam(Point(0.1f, -1.5f, 0.225f), Vector(0, 1, -0.5f), Vector(0, 0, 1), 1.0f, 1.0f);
    RayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);

    world.scene = proceduralTextures();
    engine.render(img);
    img.writePNG("tx-1.png");

    world.scene = imageTexturesBorder();
    engine.render(img);
    img.writePNG("tx-2.png");

    world.scene = imageTexturesInterpolate();
    engine.render(img);
    img.writePNG("tx-3.png");

    return 0;
}
