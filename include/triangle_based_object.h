#ifndef TRIANGLE_BASED_OBJECT_H
#define TRIANGLE_BASED_OBJECT_H

#include "scene_object.h"
#include "triangle.h"
#include <vector>

class TriangleBasedObject : public SceneObject {
protected:
  std::vector<Triangle> triangles_;
public:
  // TODO: Move to .cc file?
  // This needs to make more sense with the z-buffer.. temp solution
  virtual float RayIntersection(Ray& ray) {
    float z_buffer = FLT_MAX;
    for (int i = 0; i < triangles_.size(); i++) {
      float z_current = triangles_[i].RayIntersection(ray);
      if (z_current < z_buffer) {
        z_buffer = z_current;
      }
    }
    return z_buffer;
  }
};

#endif // TRIANGLE_BASED_OBJECT_H
