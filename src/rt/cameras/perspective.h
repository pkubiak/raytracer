#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {
  class PerspectiveCamera : public Camera {
    public:
    Point center;
    Vector forward, up;
    float verticalOpeningAngle, horizonalOpeningAngle;

    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,   //in radians
        float horizonalOpeningAngle   //in radians
        );

    virtual Ray getPrimaryRay(float x, float y) const;

  private:
    Vector fx, fy;
  };
}



#endif
