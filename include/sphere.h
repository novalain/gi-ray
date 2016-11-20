#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include "commons.h"
#include "material.h"
#include "ray.h"
#include "intersection_point.h"

class Sphere {
private:
  float radius_;
  Vertex position_;
  Material material_;

public:
  Sphere(Vertex position, float radius, ColorDbl color);
  Sphere(Vertex position, float radius, Material material);

  float get_radius() { return radius_; }
  Vertex get_position() { return position_; }

  std::unique_ptr<IntersectionPoint> RayIntersection(Ray& ray);

  static bool SolveQuadratic(const float& a,
                             const float& b,
                             const float& c,
                             float& x0,
                             float& x1);
};

#endif // SPHERE_H
