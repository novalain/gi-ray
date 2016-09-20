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
  virtual bool RayIntersection(Ray& ray, float& z) {
    bool update_pixel_color = false;
    for (int i = 0; i < triangles_.size(); i++) {
      bool is_collision_closer_to_cam = triangles_[i].RayIntersection(ray, z);
      if (is_collision_closer_to_cam) {
        update_pixel_color = true;
      }
    }
    return update_pixel_color;
  }
};

#endif // TRIANGLE_BASED_OBJECT_H
