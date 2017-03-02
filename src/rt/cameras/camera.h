#ifndef CG1RAYTRACER_CAMERAS_CAMERA_HEADER
#define CG1RAYTRACER_CAMERAS_CAMERA_HEADER

namespace rt {

class Ray;

class Camera {
public:
    virtual Ray getPrimaryRay(float x, float y) const = 0;
};

}

#endif

