#ifndef KDTREE_TEST
#define KDTREE_TEST 123

#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <cstdio>
#include <rt/cameras/perspective.h>
#include <rt/groups/kdtree.h>
#include <rt/loaders/obj.h>
#include <rt/integrators/casting.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <tuple>

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;

using namespace std;
using namespace rt;

// Test differenct parameters of KDTree construction
tuple<float, float, int, int> kdtree_test(int max_depth, float c_trav, float c_isec){
  Image img(1920, 1280);

  KDTree::max_depth = max_depth;
  KDTree::c_trav = c_trav;
  KDTree::c_isec = c_isec;

  KDTree* scene = new KDTree();

  for(int x = -3;x<=3;x+=2)
    for(int z=-3;z<=3;z+=2)
      loadOBJ(scene, "models/", "dude.obj", nullptr, Vector(x,0,z));
  // printf("%d\n", scene->primitives.size());
  auto start2 = system_clock::now();
  scene->rebuildIndex();
  auto stop2 = system_clock::now();
  auto time2 = (float)duration_cast<nanoseconds>(stop2 - start2).count()/1000000000.0;
  // printf(">> %f\n", time2);
  World world;
  world.scene = scene;

  PerspectiveCamera cam1(2.0*Point(-5.85f, 7.85f, 7.0f), Vector(1.0f,-1.0f,-1.0f), Vector(0, 1, 0), pi/8, pi/6);

  RayCastingIntegrator integrator(&world);
  Renderer engine1(&cam1, &integrator);

  auto start3 = system_clock::now();
  engine1.render(img);
  auto stop3 = system_clock::now();
  auto time3 = (float)duration_cast<nanoseconds>(stop3 - start3).count()/1000000000.0;
  // printf(">> %f\n", time3);
  int nodes = scene->nodes.size();
  int leaves = scene->leaves.size();

  for(auto p: scene->primitives)
    delete p;

  delete scene;

  return make_tuple(time2, time3, nodes, leaves);
}

void main_kdtree_test(){
  // int max_depth = 30;
  // for(int max_depth = 20; max_depth <= 50; max_depth +=5)
  // for(float c_trav = 0.5; c_trav < 5.0; c_trav += 0.75)
  //   for(float c_isec = 0.5; c_isec < 10.0; c_isec += 0.75){
  //     bool ok = true;
  //
  //     printf("%d, %.2f, %.2f, ", max_depth, c_trav, c_isec);
  //     float v0=0, v1=0, v2=0, v3=0;
  //
  //     for(int i=0;i<4;i++){
  //       std::future<tuple<float, float,int,int>> future = std::async(std::launch::async, [=](){
  //         auto res =  kdtree_test(max_depth, c_trav, c_isec);
  //         return res;
  //       });
  //
  //       auto status = future.wait_for(std::chrono::seconds(40));
  //
  //       if (status == std::future_status::deferred) {
  //           printf("deferred\n");ok=false;break;
  //       } else if (status == std::future_status::timeout) {
  //           printf("timeout\n");ok=false;break;
  //       } else if (status == std::future_status::ready) {
  //           auto res = future.get();
  //           v0+=get<0>(res);v1+=get<1>(res);v2+=get<2>(res);v3+=get<3>(res);
  //           // printf("%.2f, %.2f, %.2f, %d, %d\n", get<0>(res), get<1>(res), get<0>(res)+get<1>(res),get<2>(res),get<3>(res));
  //       }
  //     }
  //     if(ok)
  //       printf("%.2f, %.2f, %.2f, %.2f, %.2f\n",v0/4.0, v1/4.0, (v0+v1)/4.0, v2/4.0, v3/4.0);
  //   }
}
#endif
