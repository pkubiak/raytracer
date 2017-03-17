#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>

namespace rt {

  class Ray;

  class BBox {
  public:
    Point min, max;

    BBox() {}
    BBox(const Point& _min, const Point& _max) : min(rt::min(_min, _max)), max(rt::max(_min, _max)){}
    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const { return max - min; }

    std::pair<float,float> intersect(const Ray& ray) const;

    bool isUnbound();
    bool isEmpty() const;

    bool contains(Point&) const;
  };

}


#endif
