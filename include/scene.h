#ifndef SCENE_H
#define SCENE_H

#include "scene_object.h"
#include "triangle_based_object.h"
#include "triangle_custom_shape.h"
#include <memory>
#include <vector>

class Scene {
private:
  std::vector<std::unique_ptr<SceneObject>> scene_objects_;
  std::vector<std::unique_ptr<SceneObject>> scene_lights_;

  void InitRoom();
  void InitObjects();
  void InitLights();
public:
  Scene();

  const std::vector<std::unique_ptr<SceneObject>>& get_objects() const {
    return scene_objects_;
  }
};

#endif // SCENE_H
