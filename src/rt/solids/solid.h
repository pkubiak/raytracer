#pragma once

#include <rt/primitive.h>
#include <rt/bbox.h>

namespace rt {
  class Material;
  class CoordMapper;

  /*
  A solid is a physical primitive that has material and can be textured
  */
  class Solid : public Primitive {
  public:
      Material* material;
      CoordMapper* texMapper;

      explicit Solid(CoordMapper* texMapper = nullptr, Material* material = nullptr);
      virtual Point sample() const = 0;
      virtual float getArea() const = 0;
      virtual void setMaterial(Material* m) { material = m; }
      virtual void setCoordMapper(CoordMapper* cm) { texMapper = cm; }
  };

}
