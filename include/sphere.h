#ifndef SPHERE_H
#define SPHERE_H

#include "scene_object.h"

class Sphere : public SceneObject {
private:
  float radius_;

public:
  Sphere(Vertex position, float radius, ColorDbl color);

  float get_radius() { return radius_; }

  virtual bool RayIntersection(Ray& ray, float& z);
};

#endif // SPHERE_H
