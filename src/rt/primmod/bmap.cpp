#include "bmap.h"
#include <rt/bbox.h>
#include <rt/solids/triangle.h>
#include <core/color.h>
#include <rt/textures/texture.h>
#include <core/matrix.h>
#include <core/assert.h>
#include <cstdio>

namespace rt{
  BumpMapper::BumpMapper(Triangle* t, Texture* _bumpmap, const Point& _bv1, const Point& _bv2, const Point& _bv3, float _vscale):
    base(t), bumpmap(_bumpmap), bv1(_bv1), bv2(_bv2), bv3(_bv3), vscale(_vscale) {

    // (1-u-v, u, v)
    // Convert barycentric to texture, Mt*p_bary = p_text
    Matrix Mt = Matrix(
      Float4(bv1.x, bv2.x, bv3.x, 0),
      Float4(bv1.y, bv2.y, bv3.y, 0),
      Float4(    0,     0,     0, 1),
      Float4(    1,     1,     1, 0)
    );

    assert(Mt.det() > 0.00001);

    // Convert barycentric to world, Mw*p_bary = p_world
    Matrix Mw = Matrix(
      Float4(t->v1.x,t->v2.x,t->v3.x,0),
      Float4(t->v1.y,t->v2.y,t->v3.y,0),
      Float4(t->v1.z,t->v2.z,t->v3.z,0),
      Float4(0,0,0,1)
    );
    assert(Mw.det() != 0.0);

    Matrix Mti = Mt.invert();
    assert(Mti != Matrix::zero());

    // Mt.inv * p_text = Mw.inv*p_world
    // (Mw * Mt.inv)*p_text = p_world
    Mt = product(Mw, Mti);

    Ex = Vector(Mt[0][0], Mt[1][0], Mt[2][0]);
    Ey = Vector(Mt[0][1], Mt[1][1], Mt[2][1]);
  }

  BBox BumpMapper::getBounds() const {
    return base->getBounds();
  }

  Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
    auto intr = base->intersect(ray, previousBestDistance);
    if(intr){
      Point local = intr.local();
      Point pt = Point(
        local.x*bv1.x + local.y*bv2.x + local.z*bv3.x,
        local.x*bv1.y + local.y*bv2.y + local.z*bv3.y,
        local.x*bv1.z + local.y*bv2.z + local.z*bv3.z
      );

      float dx = vscale*bumpmap->getColorDX(pt).r;
      float dy = vscale*bumpmap->getColorDY(pt).r;

      Vector bv = dx*Ex + dy*Ey;

      intr.normalv = (intr.normalv - bv).normalize();
    }

    return intr;
  }

  void BumpMapper::setMaterial(Material* m) {
      base->setMaterial(m);
  }

  void BumpMapper::setCoordMapper(CoordMapper* cm){
    //TODO ?
    base->setCoordMapper(cm);
  }

}

/*
T1 = (x1, y1), T2 = (X2, y2), T3 = (x3,y3)

u,v,(1-u-v)

u*(T1-T3) + v*(T2-T3) + T3

u*P1 + v*P2 + (1-u-v)*P3
*/
