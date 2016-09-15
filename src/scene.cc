#include "scene.h"
#include "commons.h"

Scene::Scene() {
  InitRoom();
  //InitObjects();
}

void Scene::InitRoom() {
  // Floor vertices
  Vertex vfC = Vertex( 5,  0, -5); // center vertex on floor
  Vertex vf1 = Vertex(-3,  0, -5);
  Vertex vf2 = Vertex( 0, -6, -5);
  Vertex vf3 = Vertex(10, -6, -5);
  Vertex vf4 = Vertex(13,  0, -5);
  Vertex vf5 = Vertex(10,  6, -5);
  Vertex vf6 = Vertex( 0,  6, -5);

  // Ceiling vertices
  Vertex vcC = Vertex( 5,  0,  5); // center vertex on ceiling
  Vertex vc1 = Vertex(-3,  0,  5);
  Vertex vc2 = Vertex( 0, -6,  5);
  Vertex vc3 = Vertex(10, -6,  5);
  Vertex vc4 = Vertex(13,  0,  5);
  Vertex vc5 = Vertex(10,  6,  5);
  Vertex vc6 = Vertex( 0,  6,  5);

  int count = 0;

  // Colors
  glm::vec3  color_floor   = COLOR_BLACK;
  glm::vec3  color_ceiling = COLOR_WHITE;
  glm::vec3  color_wall1   = COLOR_RED;
  glm::vec3  color_wall2   = COLOR_GREEN;
  glm::vec3  color_wall3   = COLOR_BLUE;
  glm::vec3  color_wall4   = COLOR_RED;
  glm::vec3  color_wall5   = COLOR_GREEN;
  glm::vec3  color_wall6   = COLOR_BLUE;
  // Floor
  triangles_[count++] = Triangle(vfC, vf6, vf1, color_floor);
  triangles_[count++] = Triangle(vfC, vf1, vf2, color_floor);
  triangles_[count++] = Triangle(vfC, vf2, vf3, color_floor);
  triangles_[count++] = Triangle(vfC, vf3, vf4, color_floor);
  triangles_[count++] = Triangle(vfC, vf4, vf5, color_floor);
  triangles_[count++] = Triangle(vfC, vf5, vf6, color_floor);

  // Ceiling
  triangles_[count++] = Triangle(vcC, vc1, vc6, color_ceiling);
  triangles_[count++] = Triangle(vcC, vc2, vc1, color_ceiling);
  triangles_[count++] = Triangle(vcC, vc3, vc2, color_ceiling);
  triangles_[count++] = Triangle(vcC, vc4, vc3, color_ceiling);
  triangles_[count++] = Triangle(vcC, vc5, vc4, color_ceiling);
  triangles_[count++] = Triangle(vcC, vc6, vc5, color_ceiling);

  /* Counter-clockwise order, starting with front */

  // Wall1
  triangles_[count++] = Triangle(vf2, vc2, vf3, color_wall1);
  triangles_[count++] = Triangle(vf3, vc2, vc3, color_wall1);

  // Wall 2
  triangles_[count++] = Triangle(vf3, vc3, vf4, color_wall2);
  triangles_[count++] = Triangle(vf4, vc3, vc4, color_wall2);

  // Wall 3
  triangles_[count++] = Triangle(vf4, vc4, vf5, color_wall3);
  triangles_[count++] = Triangle(vc4, vc5, vf5, color_wall3);

  // Wall 4
  triangles_[count++] = Triangle(vc5, vf6, vf5, color_wall4);
  triangles_[count++] = Triangle(vc5, vc6, vf6, color_wall4);

  // Wall 5
  triangles_[count++] = Triangle(vc6, vc1, vf6, color_wall5);
  triangles_[count++] = Triangle(vc1, vf1, vf6, color_wall5);

  // Wall 6
  triangles_[count++] = Triangle(vf2, vf1, vc2, color_wall6);
  triangles_[count++] = Triangle(vf1, vc1, vc2, color_wall6);
}
