#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include <memory>
#include <vector>
#include "triangle.h"
#include "sphere.h"

class Scene {
private:
  std::vector<std::unique_ptr<Triangle>> triangles_;
  std::vector<std::unique_ptr<Sphere>> spheres_;
  std::vector<std::unique_ptr<Light>> scene_lights_;

  void InitRoom();
  void InitObjects();
  void InitLights();
public:
  Scene();

  const std::vector<std::unique_ptr<Triangle>>& get_triangles() const {
    return triangles_;
  }
  
  const std::vector<std::unique_ptr<Sphere>>& get_spheres() const {
    return spheres_;
  }

  const std::vector<std::unique_ptr<Light>>& get_lights() const {
    return scene_lights_;
  }
};

#endif // SCENE_H
