#ifndef SCENE_H
#define SCENE_H

#include "scene_object.h"
#include "light.h"
#include <memory>
#include <vector>

class Scene {
private:
  std::vector<std::unique_ptr<SceneObject>> scene_objects_;
  std::vector<std::unique_ptr<Light>> scene_lights_;

  void InitRoom();
  void InitObjects();
  void InitLights();
public:
  Scene();

  const std::vector<std::unique_ptr<SceneObject>>& get_objects() const {
    return scene_objects_;
  }

  const std::vector<std::unique_ptr<Light>>& get_lights() const {
    return scene_lights_;
  }
};

#endif // SCENE_H
