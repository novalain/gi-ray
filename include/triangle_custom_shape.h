#ifndef TRIANGLE_CUSTOM_SHAPE_H
#define TRIANGLE_CUSTOM_SHAPE_H

#include "commons.h"
#include "triangle_based_object.h"
#include <vector>

class TriangleCustomShape : public TriangleBasedObject {
public:
  TriangleCustomShape(std::vector<Triangle> triangles);
};

#endif // SCENE_OBJECT_H
