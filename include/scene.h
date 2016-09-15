#ifndef SCENE_H
#define SCENE_H

#include "triangle.h"
#include <array>

#define NUM_TRIANGLES 24

class Scene {
private:
  // TODO(nova): We want this more abstract with sceneobjects that has triangles
  // TODO 2: Do we need pointers? Will the triangles change over time?
  std::array<Triangle, NUM_TRIANGLES> triangles_;
  void InitRoom();
  //void InitObjects();

public:
  Scene();
  std::array<Triangle, NUM_TRIANGLES> get_triangles() { return triangles_; }
};

#endif // SCENE_H
