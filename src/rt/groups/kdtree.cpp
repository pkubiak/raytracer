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
#define DEBUG 0
using namespace std;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
// #define SPLIT_IN_MIDDLE 0
// #define SURFACE_AREA_HEURISTIC 1

namespace rt{
  KDTree::KDTree() {}

  KDTree::~KDTree() {
    for(auto p: nodes)
      delete p;

    for(auto p: leaves)
      delete p;
  }

  BBox KDTree::getBounds() const {
    return root_box;
  }

  tuple<int, float, float> stack[100];
  int s_pos;
  #pragma omp threadprivate(stack, s_pos)

  Intersection KDTree::intersect(const Ray& ray, float previousBestDistance) const {
    int ile = 0;
    // stack<tuple<int, float, float>> S;//node int, node bbox, time_in, time_out
    s_pos = 0;
    pair<float, float> t0t1 = root_box.intersect(ray);

    if(t0t1.first > previousBestDistance || t0t1.second < 0){
      return Intersection::failure();
    }

    stack[s_pos++] = make_tuple(0, t0t1.first, t0t1.second);

    int node_id;
    float t0, t1;
    KDTreeNode *node;

    while(s_pos > 0){
      std::tie(node_id, t0, t1) = stack[--s_pos];
      assert(t0<t1);
      node = nodes[node_id];
      if(t0 > previousBestDistance)
        continue;

      if(node->is_leaf()){
        Intersection bestIntersection = Intersection::failure();

        auto childrens = leaves[node->pointer];

        // intersect all childrens
        for(Primitive* primitive: *childrens){
          Intersection inter = primitive->intersect(ray, previousBestDistance);
          ile++;

          if(inter && inter.distance >= 0.0 && (!bestIntersection || inter.distance < bestIntersection.distance)){
            bestIntersection = inter;
            previousBestDistance = inter.distance;
          }
        }

        if(bestIntersection && bestIntersection.distance <= t1)
          return bestIntersection;

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


  // template <typename T>
  // void radixsort(vector<T>& in, vector<T>& out, unsigned char (*key)(T)){
  //   // int
  //
  // }

  #define LEFT 0
  #define RIGHT 2
  #define BOTH 1
  #define PLANAR 1

  #define TYPE_END 0
  #define TYPE_ON 1
  #define TYPE_BEGIN 2

  #define _p(X) ((*X).first)
  #define _axe(X) (((*X).second >> 28)&3)
  #define _type(X) (((*X).second >> 30)&3)
  #define _pointer(X) (((*X).second)&((1<<28)-1))

  tuple<int, vector<pair<float, unsigned> >*, BBox, int> S[105];
  int S_pos;

  void KDTree::rebuildIndex(){
      // Traversal cost and intersection cost;
      float c_trav = 1.0, c_isec = 4.0;

      // temporary vector used in spliting
      vector<char> tmp;
      tmp.resize(primitives.size());

      // float    -> split plane position
      // unsigned -> 2bits - event type(0-end, 1-planar-lying on split plane, 2-start), 2bits - axis(0-x,1-y,2-z), 28bits - primitive id.
      vector<pair<float, unsigned>>* events = new vector<pair<float, unsigned>>();
      events->resize(6*primitives.size());

      int pos = 0;
      root_box = BBox::empty();

      // Generate all (start, end, planar) events.
      for(int i=0;i<primitives.size();i++){
        BBox box = primitives[i]->getBounds();
        root_box.extend(box);

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
      sort(events->begin(), events->end());

      // Remove recursion using stack
      // std::stack<tuple<int, vector<pair<float, unsigned> >*, BBox> > S;

      // Initial KDTreeNode
      KDTreeNode *node = new KDTreeNode();
      node->pointer = 0;

      nodes.push_back(node);
      S[S_pos++] = make_tuple(0, events, root_box, primitives.size());

      while(S_pos > 0){
        node = nodes[std::get<0>(S[S_pos-1])];
        events = std::get<1>(S[S_pos-1]);
        BBox box = std::get<2>(S[S_pos-1]);
        int N = std::get<3>(S[S_pos-1]);

        int depth = node->pointer;
        float sizes[3] = {box.max.x-box.min.x, box.max.y-box.min.y, box.max.z-box.min.z};
        float SA = sizes[0]*sizes[1] + sizes[1]*sizes[2] + sizes[2]*sizes[0];

        // printf("node_id=%d, events_count=%d, stack_size=%d, depth=%d\n", std::get<0>(S.top()), events->size(), S.size(), depth);
        S_pos--;

        bool done = false;

        // printf("[%f,%f]x[%f,%f]x[%f,%f]\n", box.min.x, box.max.x, box.min.y, box.max.y, box.min.z, box.max.z);
        if(depth < 30){
          //Initialize Nl, Np, Nr
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
            if(p <= box.min(axe) || box.max(axe) <= p)
              continue;

            counts[axe][RIGHT] -= tmp_counts[TYPE_END] + tmp_counts[TYPE_ON];
            counts[axe][PLANAR] = tmp_counts[TYPE_ON];

            // compute costs assuming that we join Np, with Nl or Nr
            float v1 = sizes[(axe+1)%3];
            float v2 = sizes[(axe+2)%3];

            float SL = (p-box.min(axe))*(v1+v2)+v1*v2;
            float SR = (box.max(axe)-p)*(v1+v2)+v1*v2;

            float cost_l = (c_trav + c_isec*(SL*(counts[axe][LEFT] + counts[axe][PLANAR]) + SR*counts[axe][RIGHT])/SA);
            if((counts[axe][LEFT] + counts[axe][PLANAR]) == 0 || counts[axe][RIGHT] == 0)
              cost_l *= 0.8;

            float cost_r = (c_trav + c_isec*(SL*counts[axe][LEFT] + SR*(counts[axe][PLANAR] + counts[axe][RIGHT]))/SA);
            if(counts[axe][LEFT] == 0 || (counts[axe][PLANAR] + counts[axe][RIGHT]) == 0)
              cost_r *= 0.8;

            counts[axe][LEFT]+= tmp_counts[TYPE_ON] + tmp_counts[TYPE_BEGIN];
            counts[axe][PLANAR] = 0;

            // Update best cost
            if(cost_l < bestCost){
              bestCost = cost_l;bestP = p;bestAxe = axe; bestSide = LEFT;
            }
            if(cost_r < bestCost){
              bestCost = cost_r;bestP = p;bestAxe = axe; bestSide = RIGHT;
            }
          }


          // printf("bestCost=%f, bestP=%f, bestAxe=%d, bestSide=%d\n", bestCost, bestP, bestAxe, bestSide);

          if(bestCost + 20.0f < N*c_isec){ // subdivision
            int counts[3] = {0,0,0};
            int Nl = 0, Nr = 0; // number of primitives in left & right;

            for(auto it = events->begin(); it != events->end(); it++){
              if(_axe(it)!=bestAxe)
                continue;

              int type = _type(it);
              if(type == TYPE_END && _p(it) < bestP){
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
          node->axe = 3;

          vector<Primitive*> *node_primitives = new vector<Primitive*>();
          for(auto it = events->begin(); it != events->end(); it++)
            if(_axe(it) == 0 && _type(it) != TYPE_END)
              node_primitives->push_back(primitives[_pointer(it)]);

          leaves.push_back(node_primitives);
          node->pointer = leaves.size()-1;

          delete events;
        }
      }


  }

  // void KDTree::rebuildIndex(){
  //   // Constants for Surgace Area Heuristics (SAH)
  //   float c_trav = 1.0, c_isec = 4.0, c_min_for_split = 20.0;
  //
  //   KDTreeNode* root = new KDTreeNode();
  //   auto list = new std::list<Primitive*>();
  //
  //   nodes.push_back(root);
  //   leaves.push_back(list);
  //
  //   root_box = BBox::empty();
  //   for(auto p: primitives){
  //     list->push_back(p);
  //     root_box.extend(p->getBounds());
  //   }
  //
  //   int depth = 0;;
  //   queue<pair<int, BBox>> Q;
  //   int incDepthAt = 0, last = 0;
  //
  //   Q.push(make_pair(0, root_box));
  //
  //   while(!Q.empty()){
  //     auto q = Q.front();Q.pop();
  //     auto node = nodes[q.first];
  //
  //     BBox box = q.second;
  //     int pointer = node->pointer;
  //     std::list<Primitive*> *lower_list = leaves[pointer];
  //
  //     float c_initial = lower_list->size()*c_isec;
  //
  //     // Surface Area Heuristic (SAH)
  //     list = lower_list;
  //     vector<pair<Point, int>> V;
  //
  //     for(auto it = list->begin(); it != list->end(); it++){
  //       BBox b = (*it)->getBounds();
  //       V.push_back(make_pair(b.min,  1));
  //       V.push_back(make_pair(b.max, -1));
  //     }
  //
  //     // find best split position
  //     float bestCost = INFINITY, bestSplit = 0;
  //     int bestAxe;
  //
  //     for(int axe = 0;axe < 3; axe++){
  //       int nl = 0, nr = list->size();
  //       sort(V.begin(), V.end(), [axe](const pair<Point,int>& a, const pair<Point,int>& b) -> bool{
  //         return a.first(axe) < b.first(axe);
  //       });
  //
  //       for(int i = 0;i<V.size();i++){
  //         if(V[i].second == 1)
  //           nl++;
  //         else nr--;
  //
  //         if(i+1<V.size() && V[i+1].first(axe) == V[i].first(axe))
  //           continue;
  //
  //         float split = V[i].first(axe);
  //         float v1 = (box.max((axe+1)%3)-box.min((axe+1)%3)), v2 = (box.max((axe+2)%3)-box.min((axe+2)%3));
  //
  //         float SL = (split - box.min(axe))*(v1+v2) + v1*v2;
  //         float SR = (box.max(axe) - split)*(v1+v2) + v1*v2;
  //         float SA = (box.max(axe) - box.min(axe))*(v1+v2) + v1*v2;
  //
  //         float cost = c_trav + c_isec*(SL/SA*nl + SR/SA*nr);
  //
  //         if(cost < bestCost){
  //           bestAxe = axe; bestSplit = split;
  //           bestCost = cost;
  //         }
  //       }
  //     }
  //     V.clear();
  //
  //     if(c_initial - bestCost > c_min_for_split){
  //     // if(bestCost < list->size()*c_)
  //       // Create nodes for childrens
  //       KDTreeNode *lower = new KDTreeNode();
  //       KDTreeNode *upper = new KDTreeNode();
  //       int pos = nodes.size();
  //       nodes.push_back(lower);nodes.push_back(upper);
  //       node->pointer = pos;
  //
  //       node->axe = bestAxe;
  //       node->split = bestSplit;
  //       lower->pointer = pointer;
  //       std::list<Primitive*> *upper_list = new std::list<Primitive*>();
  //       upper->pointer = leaves.size();
  //       leaves.push_back(upper_list);
  //
  //       BBox lower_box = BBox(box.min, box.max), upper_box = BBox(box.min, box.max);
  //
  //       // do partition
  //       int total = lower_list->size();
  //       for(auto it=lower_list->begin();it!=lower_list->end();){
  //         auto it2 = it;
  //         box = (*it)->getBounds();
  //
  //         if(box.max(node->axe) >= node->split)
  //           upper_list->push_back(*it);
  //
  //         it++;
  //
  //         if(box.min(node->axe) > node->split)
  //           lower_list->erase(it2);
  //       }
  //
  //       // update bounding boxes
  //       if(node->axe == 0){
  //         lower_box.max.x = min(node->split, lower_box.max.x);
  //         upper_box.min.x = max(node->split, upper_box.min.x);
  //       }
  //       if(node->axe == 1){
  //         lower_box.max.y = min(node->split, lower_box.max.y);
  //         upper_box.min.y = max(node->split, upper_box.min.y);;
  //       }
  //       if(node->axe == 2){
  //         lower_box.max.z = min(node->split, lower_box.max.z);;
  //         upper_box.min.z = max(node->split, upper_box.min.z);
  //       }
  //
  //       Q.push(make_pair((int)node->pointer, lower_box));
  //       Q.push(make_pair((int)node->pointer+1, upper_box));
  //       last = (int)node->pointer+1;
  //     }
  //
  //     if(incDepthAt == q.first){
  //       depth++;
  //       if(depth >= 50)break;
  //       incDepthAt = last;
  //     }
  //   }
  // }

  void KDTree::add(Primitive* p){
    primitives.push_back(p);
  }

  void KDTree::setMaterial(Material* m){
    NOT_IMPLEMENTED;
  }

  void KDTree::setCoordMapper(CoordMapper* cm){
    NOT_IMPLEMENTED;
  }

  KDTreeNode::KDTreeNode():axe(3), pointer(0), split(0.0){}

  bool KDTreeNode::is_leaf() const {
    return axe == 3;
  }
}
