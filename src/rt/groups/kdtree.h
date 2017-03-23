#ifndef CG1RAYTRACER_GROUPS_KDTREE_HEADER
#define CG1RAYTRACER_GROUPS_KDTREE_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <list>
using namespace std;
namespace rt {

  // Helper class for KDTree to store division tree.
  class KDTreeNode {

  public:
    //axis: 0->x, 1->y, 2->z, 3 -> leaf
    // pointer: is_leaf? -> to leaves array, othervise -> to nodes array;
    unsigned int axe:2, pointer: 30;

    float split;

    KDTreeNode();
    ~KDTreeNode() {};
    bool is_leaf() const;
    // BBox getBounds() const;
  };

  class KDTree : public Group {
  private:

  public:
    vector<Primitive* > primitives;
    vector<KDTreeNode*> nodes;
    vector<list<Primitive*>* > leaves;
    // vector<vector<Primitive*>* > leaves;
    BBox root_box;
      KDTree();
      virtual BBox getBounds() const;
      virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
      virtual void rebuildIndex();
    	virtual ~KDTree();
      virtual void add(Primitive* p);
      virtual void setMaterial(Material* m);
      virtual void setCoordMapper(CoordMapper* cm);

  };



}


#endif
