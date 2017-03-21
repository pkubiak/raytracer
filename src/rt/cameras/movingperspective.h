#ifndef CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER
#define CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>
#include "perspective.h"

namespace rt {

class MovingPerspectiveCamera : public Camera {
  private:
    //Vector sx, sy;
    float scaleX, scaleY;
    Point (*position)(float), (*look_at)(float);
    Vector (*up)(float);
    float t;
    PerspectiveCamera *camera;

  public:
    MovingPerspectiveCamera(
      Point (*position)(float),
      Point (*look_at)(float),
      Vector (*up)(float),
      float scaleX,
      float scaleY
    );

    void setTime(float t);

    virtual Ray getPrimaryRay(float x, float y) const;
};

}


#endif
