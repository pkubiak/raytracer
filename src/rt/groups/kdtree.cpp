#include "kdtree.h"
#include <core/assert.h>
#include <list>
#include <queue>
#include <stack>
#include "../ray.h"
#include "../intersection.h"
#include <tuple>
#define DEBUG 0
using namespace std;

// #define SPLIT_IN_MIDDLE 0
// #define SURFACE_AREA_HEURISTIC 1

namespace rt{
  KDTree::KDTree() {}

  KDTree::~KDTree() {
    for(KDTreeNode* p: nodes)
      delete p;

    for(list<Primitive*>* p: leaves)
      delete p;
  }

  BBox KDTree::getBounds() const {
    return root_box;
  }

  Intersection KDTree::intersect(const Ray& ray, float previousBestDistance) const {
    int ile = 0;
    stack<tuple<int, float, float>> S;//node int, node bbox, time_in, time_out

    pair<float, float> t0t1 = root_box.intersect(ray);

    if(t0t1.first > previousBestDistance || t0t1.second < 0){
      return Intersection::failure();
    }

    S.push(make_tuple(0, t0t1.first, t0t1.second));

    int node_id;
    float t0, t1;
    KDTreeNode *node;

    while(!S.empty()){
      std::tie(node_id, t0, t1) = S.top();
      S.pop();
      assert(t0<t1);
      node = nodes[node_id];

      if(t0 > previousBestDistance)
        continue;

      if(node->is_leaf()){
        Intersection bestIntersection = Intersection::failure();

        list<Primitive*> *childrens = leaves[node->pointer];

        // intersect all childrens
        for(Primitive* primitive: *childrens){
          Intersection inter = primitive->intersect(ray, previousBestDistance);
          ile++;

          if(inter && inter.distance >= 0.0 && (!bestIntersection || inter.distance < bestIntersection.distance)){
            bestIntersection = inter;
            // printf("TAK\n");
            previousBestDistance = inter.distance;
          }
        }

        if(bestIntersection && bestIntersection.distance <= t1)
          return bestIntersection;

      }else{
        // o(axe)+t*d(axe) = split
        float t = (node->split - ray.o(node->axe))/ray.d(node->axe);

        // we intersect both subnodes
        if(t0 < t && t < t1){
          if(ray.o(node->axe) < node->split){
            S.push(make_tuple((int)node->pointer+1, t, t1));
            S.push(make_tuple((int)node->pointer, t0, t));
          }else{
            S.push(make_tuple((int)node->pointer, t, t1));
            S.push(make_tuple((int)node->pointer+1, t0, t));
          }
        }else{
          //we only intersect one subnode
          if(ray.o(node->axe) < node->split){
            if(0.0 < t && t < t0)
              S.push(make_tuple((int)node->pointer+1, t0, t1));
            else
              S.push(make_tuple((int)node->pointer, t0, t1));
          }else{
            if(0.0 < t && t < t0)
              S.push(make_tuple((int)node->pointer, t0, t1));
            else
              S.push(make_tuple((int)node->pointer+1, t0, t1));
          }
        }
      }
    }

    return Intersection::failure();
  }

  void KDTree::rebuildIndex(){
    // Constants for Surgace Area Heuristics (SAH)
    float c_trav = 1.0, c_isec = 4.0, c_min_for_split = 20.0;

    KDTreeNode* root = new KDTreeNode();
    auto list = new std::list<Primitive*>();

    nodes.push_back(root);
    leaves.push_back(list);

    for(auto p: primitives){
      list->push_back(p);
      root_box.extend(p->getBounds());
    }

    int depth = 0;
    queue<pair<int, BBox>> Q;
    int incDepthAt = 0, last = 0;

    Q.push(make_pair(0, root_box));

    while(!Q.empty()){
      auto q = Q.front();Q.pop();
      auto node = nodes[q.first];

      BBox box = q.second;
      int pointer = node->pointer;
      std::list<Primitive*> *lower_list = leaves[pointer];

      float c_initial = lower_list->size()*c_isec;

      // Surface Area Heuristic (SAH)
      list = lower_list;
      vector<pair<Point, int>> V;

      for(auto it = list->begin(); it != list->end(); it++){
        BBox b = (*it)->getBounds();
        V.push_back(make_pair(b.min,  1));
        V.push_back(make_pair(b.max, -1));
      }

      // find best split position
      float bestCost = INFINITY, bestSplit = 0;
      int bestAxe;

      for(int axe = 0;axe < 3; axe++){
        int nl = 0, nr = list->size();
        sort(V.begin(), V.end(), [axe](const pair<Point,int>& a, const pair<Point,int>& b) -> bool{
          return a.first(axe) < b.first(axe);
        });

        for(int i = 0;i<V.size();i++){
          if(V[i].second == 1)
            nl++;
          else nr--;

          if(i+1<V.size() && V[i+1].first(axe) == V[i].first(axe))
            continue;

          float split = V[i].first(axe);
          float v1 = (box.max((axe+1)%3)-box.min((axe+1)%3)), v2 = (box.max((axe+2)%3)-box.min((axe+2)%3));

          float SL = (split - box.min(axe))*(v1+v2) + v1*v2;
          float SR = (box.max(axe) - split)*(v1+v2) + v1*v2;
          float SA = (box.max(axe) - box.min(axe))*(v1+v2) + v1*v2;

          float cost = c_trav + c_isec*(SL/SA*nl + SR/SA*nr);

          if(cost < bestCost){
            bestAxe = axe; bestSplit = split;
            bestCost = cost;
          }
        }
      }
      V.clear();

      if(c_initial - bestCost > c_min_for_split){
        // Create nodes for childrens
        KDTreeNode *lower = new KDTreeNode();
        KDTreeNode *upper = new KDTreeNode();
        int pos = nodes.size();
        nodes.push_back(lower);nodes.push_back(upper);
        node->pointer = pos;

        node->axe = bestAxe;
        node->split = bestSplit;
        lower->pointer = pointer;
        std::list<Primitive*> *upper_list = new std::list<Primitive*>();
        upper->pointer = leaves.size();
        leaves.push_back(upper_list);

        BBox lower_box = BBox(box.min, box.max), upper_box = BBox(box.min, box.max);

        // do partition
        int total = lower_list->size();
        for(auto it=lower_list->begin();it!=lower_list->end();){
          auto it2 = it;
          box = (*it)->getBounds();

          if(box.max(node->axe) >= node->split)
            upper_list->push_back(*it);

          it++;

          if(box.min(node->axe) > node->split)
            lower_list->erase(it2);
        }

        // update bounding boxes
        if(node->axe == 0){
          lower_box.max.x = min(node->split, lower_box.max.x);
          upper_box.min.x = max(node->split, upper_box.min.x);
        }
        if(node->axe == 1){
          lower_box.max.y = min(node->split, lower_box.max.y);
          upper_box.min.y = max(node->split, upper_box.min.y);;
        }
        if(node->axe == 2){
          lower_box.max.z = min(node->split, lower_box.max.z);;
          upper_box.min.z = max(node->split, upper_box.min.z);
        }

        Q.push(make_pair((int)node->pointer, lower_box));
        Q.push(make_pair((int)node->pointer+1, upper_box));
        last = (int)node->pointer+1;
      }

      if(incDepthAt == q.first){
        depth++;
        if(depth >= 50)break;
        incDepthAt = last;
      }
    }
  }

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
