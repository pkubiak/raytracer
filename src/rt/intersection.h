#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Solid;

class Intersection {
  public:
    float distance;
    Ray ray;
    //Solid* solid;
    const Solid* solid;
    Vector _normal;

    Intersection() {}
    static Intersection failure();
    Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv);
    // Intersection(Intersection&);

    Point hitPoint() const;
    Vector normal() const;
    Point local() const;

    operator bool(); //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection
};

}

#endif
