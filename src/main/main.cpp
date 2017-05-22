
#include <core/image.h>
#include <rt/cameras/perspective.h>
#include <rt/groups/group.h>
#include <rt/groups/kdtree.h>
#include <rt/groups/simplegroup.h>
#include <rt/primmod/instance.h>
#include <rt/integrators/casting.h>
#include <rt/world.h>
#include <rt/renderer.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>


using namespace rt;

void demo_projective_light();

int main(){
  demo_projective_light();
}
