#pragma once

#include <vector>
#include <rt/primitive.h>
#include <rt/bbox.h>

namespace rt {
  /*
  A group is a container of multiple primitives.
  */
  class Group : public Primitive {
  public:
    BBox bbox;

    Group(): bbox(BBox::empty()){}
    virtual void rebuildIndex() = 0;
    virtual void add(Primitive* p) = 0;
  };

}
