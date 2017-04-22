#include "kdtree.h"
#include <core/assert.h>
#include <list>
#include <queue>
#include <stack>
#include <array>
#include "../ray.h"
#include "../intersection.h"
#include <tuple>
#include <chrono>

#define LEFT 0
#define RIGHT 2
#define BOTH 1
#define PLANAR 1
#define LEAF 3

#define TYPE_END 0
#define TYPE_ON 1
#define TYPE_BEGIN 2

#define _p(X) ((*X).first)
#define _axe(X) (((*X).second >> 28)&3)
#define _type(X) (((*X).second >> 30)&3)
#define _pointer(X) (((*X).second)&((1<<28)-1))

using namespace std;

namespace rt{
  // Traversal cost and intersection cost;
  float KDTree::c_trav = 1.0;
  float KDTree::c_isec = 4.0;
  int KDTree::max_depth = 30;

  KDTree::KDTree(): Group(){}

  KDTree::~KDTree() {
    for(auto p: nodes)
      delete p;

    for(auto p: leaves)
      delete p;
  }

  BBox KDTree::getBounds() const {
    return bbox;
  }

  tuple<int, float, float> stack[100];
  int s_pos;
  #pragma omp threadprivate(stack, s_pos)

  Intersection KDTree::intersect(const Ray& ray, float previousBestDistance) const {
    int ile = 0;
    s_pos = 0;
    pair<float, float> t0t1 = bbox.intersect(ray);

    if(t0t1.first > previousBestDistance || t0t1.second < 0){
      return Intersection::failure();
    }

    if(t0t1.first < 0.0)
      t0t1.first = 0.0;

    stack[s_pos++] = make_tuple(0, t0t1.first, t0t1.second);

    int node_id;
    float t0, t1;
    KDTreeNode *node;

    Intersection bestIntersection = Intersection::failure();

    while(s_pos > 0){
      std::tie(node_id, t0, t1) = stack[--s_pos];
      assert(t0<t1);
      node = nodes[node_id];

      if(t0 > previousBestDistance)
        continue;

      if(node->axe == LEAF){
        if(node->pointer > 0){
          auto childrens = leaves[node->pointer-1];

          // intersect all childrens
          for(Primitive* primitive: *childrens){
            Intersection inter = primitive->intersect(ray, previousBestDistance);
            ile++;

            if(inter && inter.distance >= 0.0 && (!bestIntersection || inter.distance < bestIntersection.distance)){
              bestIntersection = inter;
              previousBestDistance = inter.distance;
            }
          }

          if(bestIntersection && bestIntersection.distance <= t1){
            return bestIntersection;
          }
        }
      }else{
        // o(axe)+t*d(axe) = split
        float t = (node->split - ray.o(node->axe))/ray.d(node->axe);

        // we intersect both subnodes
        if(t0 <= t && t <= t1){
          if(ray.o(node->axe) < node->split){
            stack[s_pos++] = make_tuple((int)node->pointer+1, t, t1);
            stack[s_pos++] = make_tuple((int)node->pointer, t0, t);
          }else{
            stack[s_pos++] = make_tuple((int)node->pointer, t, t1);
            stack[s_pos++] = make_tuple((int)node->pointer+1, t0, t);
          }
        }else{
          //we only intersect one subnode
          if(ray.o(node->axe) < node->split){
            if(0.0 <= t && t < t0)
              stack[s_pos++] = make_tuple((int)node->pointer+1, t0, t1);
            else
              stack[s_pos++] = make_tuple((int)node->pointer, t0, t1);
          }else{
            if(0.0 <= t && t < t0)
              stack[s_pos++] = make_tuple((int)node->pointer, t0, t1);
            else
              stack[s_pos++] = make_tuple((int)node->pointer+1, t0, t1);
          }
        }
      }
    }

    return Intersection::failure();
  }


  // Based on: On building fast kd-Trees for Ray Tracing, and on doing that in O(N log N)
  //   by Ingo Wald & Vlastimil Havran
  void KDTree::rebuildIndex(){
      tuple<int, vector<pair<float, unsigned> >*, BBox, int> S[105];
      int S_pos=0;

      // temporary vector used in spliting
      vector<char> tmp;
      tmp.resize(primitives.size());

      // float    -> split plane position
      // unsigned -> 2bits - event type(0-end, 1-planar-lying on split plane, 2-start), 2bits - axis(0-x,1-y,2-z), 28bits - primitive id.
      vector<pair<float, unsigned>>* events = new vector<pair<float, unsigned>>();
      events->resize(6*primitives.size());

      int pos = 0;

      // Generate all (start, end, planar) events.
      for(int i=0;i<primitives.size();i++){
        BBox box = primitives[i]->getBounds();

        for(int axe = 0; axe < 3; axe++){
          if(box.min(axe) == box.max(axe)) // primitive lying on split plane
            (*events)[pos++] = make_pair(box.min(axe), (TYPE_ON<<30) | (axe << 28) | i);
          else{
            (*events)[pos++] = make_pair(box.min(axe), (TYPE_BEGIN<<30) | (axe << 28) | i);
            (*events)[pos++] = make_pair(box.max(axe), (TYPE_END<<30) | (axe << 28) | i);
          }
        }
      }
      events->resize(pos);

      //TODO: radix sort
      // auto start2 = system_clock::now();
      sort(events->begin(), events->end());
      // auto stop2 = system_clock::now();
      // printf("sorting: %f\n", (float)duration_cast<nanoseconds>(stop2 - start2).count()/1000000000.0);

      // Initial KDTreeNode
      KDTreeNode *node = new KDTreeNode();
      node->pointer = 0;

      nodes.push_back(node);
      S[S_pos++] = make_tuple(0, events, bbox, primitives.size());

      while(S_pos > 0){
        int node_id = std::get<0>(S[S_pos-1]);
        node = nodes[node_id];
        events = std::get<1>(S[S_pos-1]);
        BBox box = std::get<2>(S[S_pos-1]);
        int N = std::get<3>(S[S_pos-1]);

        int depth = node->pointer;
        float sizes[3] = {box.max.x-box.min.x, box.max.y-box.min.y, box.max.z-box.min.z};
        float SA = sizes[0]*sizes[1] + sizes[1]*sizes[2] + sizes[2]*sizes[0];

        S_pos--;

        bool done = false;

        if(depth < KDTree::max_depth){
          // //Initialize Nl, Np, Nr
          int counts[3][3];//Nl, Np, Nr for each axies
          counts[0][LEFT] = counts[1][LEFT] = counts[2][LEFT] = 0;
          counts[0][PLANAR] = counts[1][PLANAR] = counts[2][PLANAR] = 0;
          counts[0][RIGHT] = counts[1][RIGHT] = counts[2][RIGHT] = N;

          float bestP = 0.0, bestCost = INFINITY; int bestAxe = 0, bestSide = 0;

          // alalyze all events, find best spliting plane
          int tmp_counts[3];
          for(auto it = events->begin(); it != events->end();){
            float p = _p(it);
            int axe = _axe(it);
            tmp_counts[0] = tmp_counts[1] = tmp_counts[2] = 0;

            // Count number of event for axe=axe and value=p
            for(int i = 0; i<3;i++)
              while(it != events->end() && _p(it) == p && _axe(it) == axe && _type(it) == i){
                tmp_counts[i]++;
                tmp[_pointer(it)] = BOTH; // 0-both, 1-left, 2-right
                it++;
              }
            counts[axe][RIGHT] -= tmp_counts[TYPE_END] + tmp_counts[TYPE_ON];
            counts[axe][PLANAR] = tmp_counts[TYPE_ON];

            if(p > box.min(axe) && box.max(axe) > p){
              // compute costs assuming that we join Np, with Nl or Nr
              float v1 = sizes[(axe+1)%3];
              float v2 = sizes[(axe+2)%3];

              float SL = (p-box.min(axe))*(v1+v2)+v1*v2;
              float SR = (box.max(axe)-p)*(v1+v2)+v1*v2;

              float cost_l = (KDTree::c_trav + KDTree::c_isec*(SL*(counts[axe][LEFT] + counts[axe][PLANAR]) + SR*counts[axe][RIGHT])/SA);
              if((counts[axe][LEFT] + counts[axe][PLANAR]) == 0 || counts[axe][RIGHT] == 0)
                cost_l *= 0.8;

              float cost_r = (KDTree::c_trav + KDTree::c_isec*(SL*counts[axe][LEFT] + SR*(counts[axe][PLANAR] + counts[axe][RIGHT]))/SA);
              if(counts[axe][LEFT] == 0 || (counts[axe][PLANAR] + counts[axe][RIGHT]) == 0)
                cost_r *= 0.8;

              // Update best cost
              if(cost_l < bestCost){
                bestCost = cost_l;bestP = p;bestAxe = axe; bestSide = LEFT;
              }
              if(cost_r < bestCost){
                bestCost = cost_r;bestP = p;bestAxe = axe; bestSide = RIGHT;
              }
            }

            counts[axe][LEFT]+= tmp_counts[TYPE_ON] + tmp_counts[TYPE_BEGIN];
            counts[axe][PLANAR] = 0;
          }


          if(bestCost < N*KDTree::c_isec){ // subdivision
            int counts[3] = {0,0,0};
            int Nl = 0, Nr = 0; // number of primitives in left & right;

            for(auto it = events->begin(); it != events->end(); it++){
              if(_axe(it)!=bestAxe)
                continue;

              int type = _type(it);
              if(type == TYPE_END && _p(it) <= bestP){
                tmp[_pointer(it)] = LEFT;
                Nl++;
              }else
              if(type == TYPE_ON){//on
                if(_p(it) < bestP || (_p(it) == bestP && bestSide == LEFT)){
                  tmp[_pointer(it)] = LEFT;
                  Nl++;
                }

                if(_p(it) > bestP || (_p(it) == bestP && bestSide == RIGHT)){
                  tmp[_pointer(it)] = RIGHT;
                  Nr++;
                }
              }else//begin
              if(type == TYPE_BEGIN && _p(it) > bestP){
                tmp[_pointer(it)] = RIGHT;
                Nr++;
              }
            }
            // printf("left=%d, both=%d, right=%d\n", Nl, N-Nl-Nr, Nr);

            for(auto it = events->begin(); it != events->end(); it++)
              counts[tmp[_pointer(it)]]++;


            // Create events list for children
            auto events_left = new vector<pair<float, unsigned>>();
            auto events_right = new vector<pair<float, unsigned>>();

            events_left->resize(counts[LEFT] + counts[BOTH]);
            events_right->resize(counts[RIGHT] + counts[BOTH]);

            // Split events into left and right events (sometimes to both).
            int pos_left = 0, pos_right = 0;
            for(auto it = events->begin(); it != events->end(); it++){
              int pointer = _pointer(it);
              if(tmp[pointer] != RIGHT)//LEFT or BOTH
                (*events_left)[pos_left++] = (*it);
              if(tmp[pointer] != LEFT)//RIGHT or BOTH
                (*events_right)[pos_right++] = (*it);
            }
            assert(pos_left == events_left->size());
            assert(pos_right == events_right->size());

            // Split bbox into two parts
            BBox left_box = box, right_box = box;
            if(bestAxe == 0)
              left_box.max.x = right_box.min.x = bestP;
            if(bestAxe == 1)
              left_box.max.y = right_box.min.y = bestP;
            if(bestAxe == 2)
              left_box.max.z = right_box.min.z = bestP;

            // Create new nodes for children
            auto left_node = new KDTreeNode();
            left_node->pointer = depth+1;
            nodes.push_back(left_node);
            auto right_node = new KDTreeNode();
            right_node->pointer = depth+1;
            nodes.push_back(right_node);

            S[S_pos++] = make_tuple(nodes.size()-2, events_left, left_box, N - Nr);
            S[S_pos++] = make_tuple(nodes.size()-1, events_right, right_box, N - Nl);

            // Finish current node
            node->pointer = nodes.size()-2;

            node->axe = bestAxe;
            node->split = bestP;

            delete events;
            done = true;
          }
        }

        if(!done){
          node->axe = LEAF;

          if(events->size() > 0){
            vector<Primitive*> *node_primitives = new vector<Primitive*>();

            for(auto it = events->begin(); it != events->end(); it++)
              if(_axe(it) == 0 && _type(it) != TYPE_END){
                node_primitives->push_back(primitives[_pointer(it)]);
              }

            leaves.push_back(node_primitives);
            node->pointer = (leaves.size()-1) + 1;
          }else
            node->pointer = 0; // NULL

          delete events;
        }
      }
  }

  void KDTree::add(Primitive* p){
    primitives.push_back(p);
    bbox.extend(p->getBounds());
  }

  void KDTree::setMaterial(Material* m){
    for(auto p: primitives)
      p->setMaterial(m);
  }

  void KDTree::setCoordMapper(CoordMapper* cm){
    NOT_IMPLEMENTED;
  }

  KDTreeNode::KDTreeNode():axe(3), pointer(0), split(0.0){}

}
