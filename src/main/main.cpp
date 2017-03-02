#include "../core/image.h"
#include "../rt/cameras/perspective.h"
#include "../rt/cameras/orthographic.h"
#include "../rt/renderer.h"

using namespace rt;

int main(int argc, char* argv[])
{
  const float deg = 3.1415926/180.0;
  Image img(300, 200);
  PerspectiveCamera cam_persp(
    Point(0,0,0),
    Vector(0,0,-0.1),
    Vector(0,1,0),
    60*deg,
    60*deg
  );

  Renderer engine(&cam_persp, NULL);
  engine.render(img);
  img.writePNG("a2-perspective.png");

  OrthographicCamera cam_orth(
    Point(0,0,0),
    Vector(0,0,-0.1),
    Vector(0,1,0),
    1.0,
    1.0
  );

  Renderer engine2(&cam_orth, NULL);
  engine2.render(img);
  img.writePNG("a2-orthographic.png");

  return 0;
}
