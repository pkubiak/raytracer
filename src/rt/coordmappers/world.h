#ifndef CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER
#define CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/intersection.h>

namespace rt {

class WorldMapper : public CoordMapper {
public:
    WorldMapper();
    explicit WorldMapper(const Float4& scale);
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif
