#ifndef CG1RAYTRACER_COORDMAPPERS_COORDMAPPER_HEADER
#define CG1RAYTRACER_COORDMAPPERS_COORDMAPPER_HEADER

#include <core/vector.h>
namespace rt {

class Intersection;

class CoordMapper {
public:
    virtual Point getCoords(const Intersection& hit) const = 0;
};

}

#endif
