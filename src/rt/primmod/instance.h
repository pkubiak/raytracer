#pragma once
#include <rt/primitive.h>
#include <core/matrix.h>

namespace rt {

class Instance : public Primitive {
  protected:
    Matrix m, minv;
    Primitive* obj;

  public:
    Instance(Primitive* content);
    Primitive* content();

    void reset(); //reset transformation back to identity
    void translate(const Vector& t);
    void rotate(const Vector& axis, float angle);
    void scale(float scale);
    void scale(const Vector& scale);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
  };
}
