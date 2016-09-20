#include "scene.h"
#include "commons.h"
#include "triangle_custom_shape.h"
#include "tetrahedron.h"
#include "sphere.h"

Scene::Scene() {
  InitObjects();
  InitRoom();
}

void Scene::InitObjects() {
  Vertex v0 = Vertex( 8,  6, -2);
  Vertex v1 = Vertex( 9,  3, -2);
  Vertex v2 = Vertex(10,  6, -2);
  Vertex v3 = Vertex( 9,  4.5f,  1);
  // Bottom
  Triangle t0 = Triangle(v0, v2, v1, COLOR_PINK);
  // Sides
  Triangle t1 = Triangle(v0, v1, v3, COLOR_MAGENTA); // "front"
  Triangle t2 = Triangle(v1, v2, v3, COLOR_YELLOW); // "back"
  Triangle t3 = Triangle(v0, v3, v2, ColorDbl(100,100,100)); // "left side"

  scene_objects_.push_back(std::make_unique<Tetrahedron>(2.5f, 3.f, Vertex(8,-2,-2)));

  scene_objects_.push_back(
      std::make_unique<Sphere>(Vertex(0, 0, 0), 2.f, COLOR_MAGENTA));
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

  // Colors
  glm::vec3  color_floor   = COLOR_WHITE;
  glm::vec3  color_ceiling = COLOR_BLACK;
  glm::vec3  color_wall1   = COLOR_RED;
  glm::vec3  color_wall2   = COLOR_GREEN;
  glm::vec3  color_wall3   = COLOR_BLUE;
  glm::vec3  color_wall4   = COLOR_CYAN;
  glm::vec3  color_wall5   = COLOR_MAGENTA;
  glm::vec3  color_wall6   = COLOR_YELLOW;

  std::vector<Triangle> triangle_list;

  // Floor
  triangle_list.push_back(Triangle(vfC, vf6, vf1, color_floor));
  triangle_list.push_back(Triangle(vfC, vf1, vf2, color_floor));
  triangle_list.push_back(Triangle(vfC, vf2, vf3, color_floor));
  triangle_list.push_back(Triangle(vfC, vf3, vf4, color_floor));
  triangle_list.push_back(Triangle(vfC, vf4, vf5, color_floor));
  triangle_list.push_back(Triangle(vfC, vf5, vf6, color_floor));

  // Ceiling
  triangle_list.push_back(Triangle(vcC, vc1, vc6, color_ceiling));
  triangle_list.push_back(Triangle(vcC, vc2, vc1, color_ceiling));
  triangle_list.push_back(Triangle(vcC, vc3, vc2, color_ceiling));
  triangle_list.push_back(Triangle(vcC, vc4, vc3, color_ceiling));
  triangle_list.push_back(Triangle(vcC, vc5, vc4, color_ceiling));
  triangle_list.push_back(Triangle(vcC, vc6, vc5, color_ceiling));

  /* Counter-clockwise order, starting with front */

  // Wall1
  triangle_list.push_back(Triangle(vf2, vc2, vf3, color_wall1));
  triangle_list.push_back(Triangle(vf3, vc2, vc3, color_wall1));

  // Wall 2
  triangle_list.push_back(Triangle(vf3, vc3, vf4, color_wall2));
  triangle_list.push_back(Triangle(vf4, vc3, vc4, color_wall2));

  // Wall 3
  triangle_list.push_back(Triangle(vf4, vc4, vf5, color_wall3));
  triangle_list.push_back(Triangle(vf5, vc4, vc5, color_wall3));

  // Wall 4
  triangle_list.push_back(Triangle(vf5, vc5, vf6, color_wall4));
  triangle_list.push_back(Triangle(vf6, vc5, vc6, color_wall4));

  // Wall 5
  triangle_list.push_back(Triangle(vf6, vc6, vf1, color_wall5));
  triangle_list.push_back(Triangle(vf1, vc6, vc1, color_wall5));

  // Wall 6
  triangle_list.push_back(Triangle(vf1, vc1, vf2, color_wall6));
  triangle_list.push_back(Triangle(vf2, vc1, vc2, color_wall6));

  scene_objects_.push_back(
      std::make_unique<TriangleCustomShape>(triangle_list));
}
