#include "instance.h"
#include <rt/primitive.h>
#include <core/assert.h>
#include <core/point.h>
#include <core/matrix.h>
#include <core/float4.h>
#include <rt/bbox.h>
#include <rt/primitive.h>
#include <rt/intersection.h>

namespace rt {
  Instance::Instance(Primitive* _content): m(Matrix::identity()), minv(Matrix::identity()), obj(_content) {}

  Primitive* Instance::content(){
    return obj;
  }

  void Instance::reset(){ //reset transformation back to identity
    m = Matrix::identity();
    minv = Matrix::identity();
  }

  void Instance::translate(const Vector& t) {
    m = product(
      Matrix(
        Float4(1, 0, 0, t.x),
        Float4(0, 1, 0, t.y),
        Float4(0, 0, 1, t.z),
        Float4(0, 0, 0,   1)
      ),
      m);
    minv = m.invert();
  }

  void Instance::rotate(const Vector& axis, float angle) {
    Vector v = axis.normalize();
    float a = angle;

    Matrix s = Matrix(
      Float4(cos(a),      0,      0, 0),
      Float4(     0, cos(a),      0, 0),
      Float4(     0,      0, cos(a), 0),
      Float4(     0,      0,      0, 1)
    );

    Matrix u = Matrix(
      Float4(   0, -v.z,  v.y, 0),
      Float4( v.z,    0, -v.x, 0),
      Float4(-v.y,  v.x,    0, 0),
      Float4(   0,    0,    0, 0)
    );

    Matrix uu = Matrix(
      Float4(v.x*v.x, v.x*v.y,v.x*v.z, 0),
      Float4(v.x*v.y, v.y*v.y,v.y*v.z, 0),
      Float4(v.x*v.z, v.y*v.z,v.z*v.z, 0),
      Float4(      0,       0,      0, 0)
    );

    m = product(s + sin(a)*u + (1.0 - cos(a))*uu, m);
    minv = m.invert();
  }

  void Instance::scale(float s) {
    m = product(
      Matrix(
        Float4(s, 0, 0, 0),
        Float4(0, s, 0, 0),
        Float4(0, 0, s, 0),
        Float4(0, 0, 0, 1)
      ),
      m);
    minv = m.invert();
  }

  void Instance::scale(const Vector& s) {
    m = product(
      Matrix(
        Float4(s.x,   0,   0, 0),
        Float4(  0, s.y,   0, 0),
        Float4(  0,   0, s.z, 0),
        Float4(  0,   0,   0, 1)
      ),
      m);
    minv = m.invert();
  }

  BBox Instance::getBounds() const {
    BBox box = obj->getBounds();
    Point a = box.min, b = box.max;

    BBox res = BBox::empty();

    res.extend(m * Point(a.x,a.y,a.z));
    res.extend(m * Point(a.x,a.y,b.z));
    res.extend(m * Point(a.x,b.y,a.z));
    res.extend(m * Point(a.x,a.y,b.z));
    res.extend(m * Point(b.x,a.y,a.z));
    res.extend(m * Point(b.x,a.y,b.z));
    res.extend(m * Point(b.x,b.y,a.z));
    res.extend(m * Point(b.x,a.y,b.z));

    return res;
  }

  Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
    Point x = minv*ray.getPoint(previousBestDistance);

    auto intr = obj->intersect(Ray(minv*ray.o, minv*ray.d), ((Vector)((Float4)x - (Float4)(minv*ray.o))).length()/ray.d.length());

    if(intr){
      Point p = m*intr.ray.getPoint(intr.distance);
      float dist = ((Vector)((Float4)p - (Float4)ray.o)).length() / ray.d.length();

      if(dist <= previousBestDistance)
        return Intersection(dist, ray, intr.solid, (minv.transpose()*intr.normalv).normalize(), intr.uv);
    }

    return Intersection::failure();
  }

  void Instance::setMaterial(Material* m) {
    NOT_IMPLEMENTED;
  }

  void Instance::setCoordMapper(CoordMapper* cm) {
    NOT_IMPLEMENTED;
  }

}
