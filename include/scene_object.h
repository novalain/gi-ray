#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "commons.h"

class Ray;
class IntersectionPoint;

class SceneObject {
public:
  virtual ~SceneObject() = default;
  virtual IntersectionPoint* RayIntersection(Ray& ray) = 0;

  Vertex get_position() { return position_; }

protected:
  SceneObject(Vertex position) { position_ = position; }
  SceneObject() = default;
  Vertex position_;
};

#endif // SCENE_OBJECT_H
