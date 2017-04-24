#ifndef CG1RAYTRACER_INTERPOLATE_HEADER
#define CG1RAYTRACER_INTERPOLATE_HEADER

namespace rt {

template <typename T>
inline T lerp(const T& px0, const T& px1, float xPoint) {
  return (px0 * (1.0f - xPoint)) + (px1 * xPoint);
}

template <typename T>
inline T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {

}

template <typename T>
inline T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xPoint, float yPoint) {
  return (px0y0 * ((1.0f - xPoint)*(1.0f - yPoint))) +
    (px1y0 * (xPoint * (1.0f-yPoint))) +
    (px0y1 * ((1.0f - xPoint) * yPoint)) +
    (px1y1 * (xPoint * yPoint));
}

template <typename T>
inline T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
         const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
         float xPoint, float yPoint, float zPoint) {
  return (px0y0z0 * ((1.0f - xPoint) * (1.0f - yPoint) * (1.0f - zPoint))) +
  (px0y0z1 * ((1.0f - xPoint) * (1.0f - yPoint) * zPoint)) +
  (px0y1z0 * ((1.0f - xPoint) * yPoint * (1.0f - zPoint))) +
  (px0y1z1 * ((1.0f - xPoint) * yPoint * zPoint)) +
  (px1y0z0 * (xPoint * (1.0f - yPoint) * (1.0f - zPoint))) +
  (px1y0z1 * (xPoint * (1.0f - yPoint) * zPoint)) +
  (px1y1z0 * (xPoint * yPoint * (1.0f - zPoint))) +
  (px1y1z1 * (xPoint * yPoint * zPoint));
}

//Point class has no + operator. You need to cast it to/from Float4 to do lerp. We add these for convenience:
class Point;

Point lerp(const Point& px0, const Point& px1, float xPoint);
Point lerpbar(const Point& px0, const Point& px1, const Point& px2, float xWeight, float yWeight);
Point lerp2d(const Point& px0y0, const Point& px1y0, const Point& px0y1, const Point& px1y1, float xPoint, float yPoint);
Point lerp3d(const Point& px0y0z0, const Point& px1y0z0, const Point& px0y1z0, const Point& px1y1z0,
    const Point& px0y0z1, const Point& px1y0z1, const Point& px0y1z1, const Point& px1y1z1,
    float xPoint, float yPoint, float zPoint);

}

//The implementation of template functions must appear in the header file, visible for everyone who uses them.
//If you want the implementation to be in a separate file, it should still included explicitly.
//Uncomment the following line once you create the file:
//#include "interpolate-impl.h"

#endif
