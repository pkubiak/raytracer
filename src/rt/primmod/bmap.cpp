#include "bmap.h"
#include <rt/bbox.h>
#include <rt/solids/triangle.h>
#include <core/color.h>
#include <rt/textures/texture.h>
#include <core/matrix.h>
#include <core/assert.h>
#include <cstdio>

namespace rt{
  BumpMapper::BumpMapper(Triangle* _base, Texture* _bumpmap, const Point& _bv1, const Point& _bv2, const Point& _bv3, float _vscale):
    base(_base), bumpmap(_bumpmap), bv1(_bv1), bv2(_bv2), bv3(_bv3), vscale(_vscale) {

    // (1-u-v, u, v)
    // Convert barycentric to texture, Mt*p_bary = p_text
    Matrix Mt = Matrix(
      Float4(bv2.x - bv1.x, bv3.x - bv1.x, 1, bv1.x),
      Float4(bv2.y - bv1.y, bv3.y - bv1.y, 1, bv1.y),
      Float4(0, 0, 1, 0),
      Float4(0, 0, 0, 1)
    );


    // Convert barycentric to world, Mw*p_bary = p_world
    Matrix Mw = Matrix(
      Float4(base->v2.x - base->v1.x, base->v3.x - base->v1.x, 1, base->v1.x),
      Float4(base->v2.y - base->v1.y, base->v3.y - base->v1.y, 1, base->v1.y),
      Float4(base->v2.z - base->v1.z, base->v3.z - base->v1.z, 1, base->v1.z),
      Float4(0,0,0,1)
    );
    // assert(Mw!= Matrix::zero());

    Matrix Mti = Mt.invert();
    assert(Mti != Matrix::zero());

    // Mt.inv * p_text = Mw.inv*p_world
    // (Mw * Mt.inv)*p_text = p_world
    Mt = product(Mw, Mti);

    // printf("Eu = [%f, %f, %f]\n", Mt[0][0], Mt[1][0], Mt[2][0], Mt[3][0]);
    // printf("Ev = [%f, %f, %f]\n", Mt[0][1], Mt[1][1], Mt[2][1], Mt[3][1]);
    // printf("Ew = [%f, %f, %f]\n", Mt[0][2], Mt[1][2], Mt[2][2], Mt[3][1]);
    // printf("\n");

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

      float dx = vscale*(bumpmap->getColorDX(pt).r);
      float dy = vscale*(bumpmap->getColorDY(pt).r);


      intr.normalv = (intr.normalv + dy*cross(intr.normalv, Ex) + dx*cross(Ey, intr.normalv)).normalize();
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
