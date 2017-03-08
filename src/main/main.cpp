#include <rt/cameras/perspective.h>
#include <rt/cameras/orthographic.h>
#include <core/image.h>
#include <core/vector.h>
#include <core/point.h>
#include <rt/renderer.h>

using namespace rt;

int main() {
    Image img(300, 200);
    {
    PerspectiveCamera cam(Point(0, 0, 10), Vector(0, 0, -1), Vector(0, 1, 0), pi/4, pi/3);
    Renderer engine(&cam, nullptr);
    engine.render(img);
    img.writePNG("a2-1.png");
    }

    {
    OrthographicCamera cam(Point(0, 0, 10), Vector(0, 0, -1), Vector(0, 1, 0), 10.0f, 10.0f);
    Renderer engine(&cam, nullptr);
    engine.render(img);
    img.writePNG("a2-2.png");
    }
    return 0;
}
