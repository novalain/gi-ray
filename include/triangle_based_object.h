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
  virtual bool RayIntersection(Ray& ray) {
    for (int i = 0; i < triangles_.size(); i++) {
      bool collision = triangles_[i].RayIntersection(ray);
      if (collision) {
        return true;
      }
    }
    return false;
  }
};

#endif // TRIANGLE_BASED_OBJECT_H
