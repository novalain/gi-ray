#include "scene.h"
#include "commons.h"
#include "tetrahedron.h"
#include "point_light.h"
#include "sphere.h"

Scene::Scene() {
  InitObjects();
  InitRoom();
  InitLights();
}

void Scene::InitObjects() {
  Vertex v0 = Vertex( 8 - 6,  6 - 7, -2);
  Vertex v1 = Vertex( 9 - 6,  3 - 7, -2);
  Vertex v2 = Vertex(10 - 6,  6 - 7, -2);
  Vertex v3 = Vertex( 9 - 6,  4.5f - 7.f,  1);

  // Bottom
  /*Triangle t0 = Triangle(v0, v2, v1, tetra1_mat);
  // Sides
  Triangle t1 = Triangle(v0, v1, v3, tetra1_mat); // "front"
  Triangle t2 = Triangle(v1, v2, v3, tetra1_mat); // "back"
  Triangle t3 = Triangle(v0, v3, v2, ColorDbl(100,100,100)); // "left side"*/

  // scene_objects_.push_back(std::make_unique<Tetrahedron>(t0, t1, t2, t3));
  //scene_objects_.push_back(std::make_unique<Tetrahedron>(3.f, 3.5f, Vertex(6, -2.f,-3.5f), GLASS_MAT));

  scene_objects_.push_back(std::make_unique<Triangle>(v0, v2, v1, Material(1,0,0, COLOR_BLUE, glm::vec3(0,0,0))));
  scene_objects_.push_back(std::make_unique<Triangle>(v0, v1, v3, Material(1,0,0, COLOR_BLUE, glm::vec3(0,0,0))));
  scene_objects_.push_back(std::make_unique<Triangle>(v1, v2, v3, Material(1,0,0, COLOR_BLUE, glm::vec3(0,0,0))));
  scene_objects_.push_back(std::make_unique<Triangle>(v0, v3, v2, Material(1,0,0, COLOR_BLUE, glm::vec3(0,0,0))));

  scene_objects_.push_back(std::make_unique<Sphere>(Vertex(6.f, -0.2f, 3.f), 1.0f, GLASS_MAT));
  scene_objects_.push_back(std::make_unique<Sphere>(Vertex(5.f, 2.5f, -2.5f), 1.6f, PERFECT_MIRROR));
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

  // Materials
  Material floor_mat = Material(1,0,0, COLOR_WHITE, glm::vec3(0,0,0));
  Material ceiling_mat = Material(1,0,0, COLOR_WHITE, glm::vec3(0,0,0));
  Material wall1_mat = Material(1,0,0, COLOR_BLUE, glm::vec3(0,0,0));
  Material wall2a_mat = Material(0,1,0, COLOR_WHITE, glm::vec3(0,0,0));
  Material wall2b_mat = Material(1,0,0, COLOR_WHITE, glm::vec3(0,0,0));
  Material wall3_mat = Material(1,0,0, COLOR_WHITE, glm::vec3(0,0,0));
  Material wall4_mat = Material(1,0,0, COLOR_RED, glm::vec3(0,0,0));
  Material wall5_mat = Material(1,0,0, COLOR_BLACK, glm::vec3(0,0,0));
  Material wall6_mat = Material(1,0,0, COLOR_BLACK, glm::vec3(0,0,0));
  Material area_light_mat = Material(1,0,0, COLOR_WHITE, glm::vec3(255, 255, 255));

  //std::vector<Triangle> triangle_list;

  // Floor
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf6, vf1, floor_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf1, vf2, floor_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf2, vf3, floor_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf3, vf4, floor_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf4, vf5, floor_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vfC, vf5, vf6, floor_mat));

  // Ceiling
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc1, vc6, ceiling_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc2, vc1, ceiling_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc3, vc2, ceiling_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc4, vc3, ceiling_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc5, vc4, ceiling_mat  ));
  scene_objects_.push_back(std::make_unique<Triangle>(vcC, vc6, vc5, ceiling_mat));

  /* Counter-clockwise order, starting with front */

  // Wall1
  scene_objects_.push_back(std::make_unique<Triangle>(vf2, vc2, vf3, wall1_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf3, vc2, vc3, wall1_mat));

  // Wall 2
  scene_objects_.push_back(std::make_unique<Triangle>(vf3, vc3, vf4, wall2b_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf4, vc3, vc4, wall2a_mat));

  // Wall 3
  scene_objects_.push_back(std::make_unique<Triangle>(vf4, vc4, vf5, wall3_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf5, vc4, vc5, wall3_mat));

  // Wall 4
  scene_objects_.push_back(std::make_unique<Triangle>(vf5, vc5, vf6, wall4_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf6, vc5, vc6, wall4_mat));

  // Wall 5
  scene_objects_.push_back(std::make_unique<Triangle>(vf6, vc6, vf1, wall5_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf1, vc6, vc1, wall5_mat));

  // Wall 6
  scene_objects_.push_back(std::make_unique<Triangle>(vf1, vc1, vf2, wall6_mat));
  scene_objects_.push_back(std::make_unique<Triangle>(vf2, vc1, vc2, wall6_mat));
}

void Scene::InitLights() {
  scene_lights_.push_back(std::make_unique<PointLight>(Vertex(5.f,0.f,4.5f), 100.f, COLOR_WHITE));
  scene_lights_.push_back(std::make_unique<PointLight>(Vertex(5.f,0.f,-3.5f), 100.f, COLOR_WHITE));
}
