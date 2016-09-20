#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "commons.h"

class Ray;

class SceneObject {
public:
  virtual ~SceneObject() = default;
  virtual bool RayIntersection(Ray& ray, float& z) = 0;

protected:
  SceneObject(Vertex position) { position_ = position; }
  SceneObject() = default;
  Vertex position_;
};

#endif // SCENE_OBJECT_H
