#include "kdtree.h"
#include <core/assert.h>
#include <list>
#include <queue>
#include <stack>
#include "../ray.h"
#include "../intersection.h"
#include <tuple>

using namespace std;

namespace rt{
  KDTree::KDTree() {}

  KDTree::~KDTree() {
    // for(auto p: nodes)
    //   delete p;
    // for(list<Primiteve*>* p: leaves){
    //   for(auto l: *p)
    //     delete l;
    //   delete p;
    // }
  }

  BBox KDTree::getBounds() const {
    return root_box;
  }

  Intersection KDTree::intersect(const Ray& ray, float previousBestDistance) const {
    int ile = 0;
    // stack<pair<int, BBox>> S;
    stack<tuple<int, float, float>> S;//node int, node bbox, time_in, time_out

    pair<float, float> t0t1 = root_box.intersect(ray);

    if(t0t1.first > previousBestDistance || t0t1.second < 0){
      // printf("FAST FAIL\n");
      return Intersection::failure();
    }

    S.push(make_tuple(0, t0t1.first, t0t1.second));

    int node_id;
    float t0, t1;
    // BBox box;
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
          // printf("%f\n", t);
          if(ray.o(node->axe) < node->split){
            // if(t<previousBestDistance)
            S.push(make_tuple((int)node->pointer+1, t, t1));
            S.push(make_tuple((int)node->pointer, t0, t));
          }else{
            // if(t<previousBestDistance)
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
    // printf("ILe: %d\n", ile);
    return Intersection::failure();
  }

  void KDTree::rebuildIndex(){
    auto root = new KDTreeNode();
    auto list = new std::list<Primitive*>();

    nodes.push_back(root);
    leaves.push_back(list);

    for(auto p: primitives){
      list->push_back(p);
      BBox b = p->getBounds();
      // printf("ROOT_BOX: [%f,%f,%f] x [%f,%f,%f]\n", b.min.x, b.min.y,b.min.z,b.max.x,b.max.y,b.max.z);
      root_box.extend(p->getBounds());
    }
    // printf("ROOT_BOX: [%f,%f,%f] x [%f,%f,%f]\n", root_box.min.x, root_box.min.y,root_box.min.z,root_box.max.x,root_box.max.y,root_box.max.z);
    int depth = 0;
    queue<pair<int, BBox>> Q;
    int incDepthAt = 0, last = 0;

    Q.push(make_pair(0, root_box));

    while(!Q.empty()){
      //printf("TAK: %d, %d\n", Q.size(), depth);
      auto q = Q.front();Q.pop();
      auto node = nodes[q.first];
      BBox box = q.second;
      int pointer = node->pointer;

      if(node->is_leaf()&&leaves[node->pointer]->size() > 3&&depth < 25){
        // printf("Leaves: %d", leaves[node->pointer]->size());
        KDTreeNode *lower = new KDTreeNode();
        KDTreeNode *upper = new KDTreeNode();
        int pos = nodes.size();
        nodes.push_back(lower);nodes.push_back(upper);
        node->pointer = pos;

        std::list<Primitive*> *lower_list = leaves[pointer];
        int initial_size = lower_list->size();

        int bestaxe=0, bestv = 123456789;
        float bestsplit = 0.0;

        // for(int i=0;i<100;i++){
        //    int lower = 0, upper = 0;
        //    int taxe = rand()%3;
        //   //  float split = 0.5*(box.max(taxe)+box.min(taxe));
        //   float r = (rand()%10000)/9999.0;
        //   float split = (r*box.max(taxe)+(1-r)*box.min(taxe));
        //
        //    for(auto it=lower_list->begin();it!=lower_list->end();it++){
        //      BBox b = (*it)->getBounds();
        //      if(b.max(taxe) >= split)upper++;
        //      if(b.min(taxe) <= split)lower++;
        //    }
        //   // upper -= lower_list->size()/2;
        //   // lower -= lower_list->size()/2;
        //   // int v = (upper<0?-upper:upper)+(lower<0?-lower:lower);
        //   // printf("%d %d %d -> %d\n",lower,upper,lower_list->size()/2, v);
        //
        //   if(lower*lower+upper*upper<bestv){
        //     bestv = lower*lower+upper*upper;bestaxe=taxe;bestsplit=split;
        //   }
        // }
        // printf("BEST AXE: %d, %d -> %f\n",bestaxe, bestv, bestsplit);

        int axe = 0;
        float x = box.max(0)-box.min(0), y = box.max(1)-box.min(1), z = box.max(2)-box.min(2);
        if(y>=x&&y>=z)axe=1;
        if(z>=y&&z>=x)axe=2;
        axe = depth%3;
        // axe = rand()%3;
        node->axe = axe;
        node->split = 0.5*(box.max(axe)+box.min(axe));
        lower->pointer = pointer;
        std::list<Primitive*> *upper_list = new std::list<Primitive*>();
        upper->pointer = leaves.size();
        leaves.push_back(upper_list);

        BBox lower_box = BBox::empty(), upper_box = BBox::empty();
        // BBox lower_box = BBox(box.min, box.max), upper_box = BBox(box.min, box.max);


        // printf("Split: axe=%d, split=%f (%f,%f,%f) %d,size:%d\n",node->axe, node->split, x,y,z, depth,initial_size);

        // do partition
        for(auto it=lower_list->begin();it!=lower_list->end();){
          auto it2 = it;
          box = (*it)->getBounds();

          if(box.max(axe) >= node->split){
            upper_box.extend(box);
            upper_list->push_back(*it);
          }
          it++;
          if(box.min(axe) <= node->split){
            // retein in list
            lower_box.extend(box);
          }else{
            // remove from list
            lower_list->erase(it2);
          }
        }

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

        // printf("Balance: %d -> %d,%d\n", initial_size, lower_list->size(), upper_list->size());
        if(lower_list->size() == initial_size || upper_list->size() == initial_size)
          continue;

        if(10*(lower_list->size()+upper_list->size()) < 14 *initial_size){
          Q.push(make_pair((int)node->pointer, lower_box));
          Q.push(make_pair((int)node->pointer+1, upper_box));
          last = (int)node->pointer + 1;
        }
      }

      if(incDepthAt == q.first){
        depth++;
        incDepthAt = last;
      }
    }

    // for(int i=0;i<leaves.size();i++)
    //   printf("%d ", leaves[i]->size());
    // printf("\n");
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

  // BBox KDTreeNode::getBounds() const {
  //   BBox box = BBox::empty();
  //   for(auto c: childrens)
  //     box.extend(c->getBounds());
  //   return box;
  // }

  bool KDTreeNode::is_leaf() const {
    return axe == 3;
  }
}
