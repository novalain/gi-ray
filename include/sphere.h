#ifndef SPHERE_H
#define SPHERE_H

#include "scene_object.h"
#include "material.h"

class Sphere : public SceneObject {
private:
  float radius_;
  Material material_;

public:
  Sphere(Vertex position, float radius, ColorDbl color);
  Sphere(Vertex position, float radius, Material material);

  float get_radius() { return radius_; }

  virtual bool RayIntersection(Ray& ray, float& z);

  static bool SolveQuadratic(const float& a,
                             const float& b,
                             const float& c,
                             float& x0,
                             float& x1);
};

#endif // SPHERE_H
