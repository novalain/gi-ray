#ifndef TRIANGLE_BASED_OBJECT_H
#define TRIANGLE_BASED_OBJECT_H

#include "scene_object.h"
#include "triangle.h"
#include <vector>

class TriangleBasedObject : SceneObject {
  protected:
    std::vector<Triangle> triangles_;
};

#endif // TRIANGLE_BASED_OBJECT_H
