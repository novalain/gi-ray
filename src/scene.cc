#include "scene.h"
#include "commons.h"
#include "tetrahedron.h"
#include "point_light.h"

Scene::Scene() {
  InitObjects();
  InitRoom();
  InitLights();
}

void Scene::InitObjects() {
  Vertex v0 = Vertex(2.f, -1.0f, -2.f);
  Vertex v1 = Vertex(3.f, -4.0f, -2.f);
  Vertex v2 = Vertex(4.f, -1.0f, -2.f);
  Vertex v3 = Vertex(3.f, -2.5f,  1.f);

  // Bottom
  /*Triangle t0 = Triangle(v0, v2, v1, tetra1_mat);
  // Sides
  Triangle t1 = Triangle(v0, v1, v3, tetra1_mat); // "front"
  Triangle t2 = Triangle(v1, v2, v3, tetra1_mat); // "back"
  Triangle t3 = Triangle(v0, v3, v2, ColorDbl(100,100,100)); // "left side"*/

  // scene_objects_.push_back(std::make_unique<Tetrahedron>(t0, t1, t2, t3));
  //scene_objects_.push_back(std::make_unique<Tetrahedron>(3.f, 3.5f, Vertex(6, -2.f,-3.5f), GLASS_MAT));

  Material green_diffuse = Material(1.f, 0.f, 0.f, COLOR_GREEN, glm::vec3(0.f, 0.f, 0.f));
  triangles_.push_back(std::make_unique<Triangle>(v0, v2, v1, green_diffuse));
  triangles_.push_back(std::make_unique<Triangle>(v0, v1, v3, green_diffuse));
  triangles_.push_back(std::make_unique<Triangle>(v1, v2, v3, green_diffuse));
  triangles_.push_back(std::make_unique<Triangle>(v0, v3, v2, green_diffuse));

  //spheres_.push_back(std::make_unique<Sphere>(Vertex(6.f, -0.2f, 3.f), 1.0f, GLASS_MAT));
  spheres_.push_back(std::make_unique<Sphere>(Vertex(5.f, 2.5f, -2.5f), 1.6f, PERFECT_MIRROR));
}

void Scene::InitRoom() {
  // Floor vertices
  Vertex vfC = Vertex( 5.f,  0.f, -5.f); // center vertex on floor
  Vertex vf1 = Vertex(-3.f,  0.f, -5.f);
  Vertex vf2 = Vertex( 0.f, -6.f, -5.f);
  Vertex vf3 = Vertex(10.f, -6.f, -5.f);
  Vertex vf4 = Vertex(13.f,  0.f, -5.f);
  Vertex vf5 = Vertex(10.f,  6.f, -5.f);
  Vertex vf6 = Vertex( 0.f,  6.f, -5.f);

  // Ceiling vertices
  Vertex vcC = Vertex( 5.f,  0.f,  5.f); // center vertex on ceiling
  Vertex vc1 = Vertex(-3.f,  0.f,  5.f);
  Vertex vc2 = Vertex( 0.f, -6.f,  5.f);
  Vertex vc3 = Vertex(10.f, -6.f,  5.f);
  Vertex vc4 = Vertex(13.f,  0.f,  5.f);
  Vertex vc5 = Vertex(10.f,  6.f,  5.f);
  Vertex vc6 = Vertex( 0.f,  6.f,  5.f);

  // Materials
  Material floor_mat    = Material(1.f, 0.f, 0.f, COLOR_WHITE, glm::vec3(0.f, 0.f, 0.f));
  Material ceiling_mat  = Material(1.f, 0.f, 0.f, COLOR_WHITE, glm::vec3(0.f, 0.f, 0.f));
  Material wall1_mat    = Material(1.f, 0.f, 0.f, COLOR_BLUE,  glm::vec3(0.f, 0.f, 0.f));
  Material wall2a_mat   = Material(1.f, 0.f, 0.f, COLOR_WHITE, glm::vec3(0.f, 0.f, 0.f));
  Material wall2b_mat   = Material(1.f, 0.f, 0.f, COLOR_WHITE, glm::vec3(0.f, 0.f, 0.f));
  Material wall3_mat    = Material(1.f, 0.f, 0.f, COLOR_WHITE, glm::vec3(0.f, 0.f, 0.f));
  Material wall4_mat    = Material(1.f, 0.f, 0.f, COLOR_RED,   glm::vec3(0.f, 0.f, 0.f));
  Material wall5_mat    = Material(1.f, 0.f, 0.f, COLOR_BLACK, glm::vec3(0.f, 0.f, 0.f));
  Material wall6_mat    = Material(1.f, 0.f, 0.f, COLOR_BLACK, glm::vec3(0.f, 0.f, 0.f));

  //std::vector<Triangle> triangle_list;

  // Floor
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf6, vf1, floor_mat));
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf1, vf2, floor_mat));
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf2, vf3, floor_mat));
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf3, vf4, floor_mat));
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf4, vf5, floor_mat));
  triangles_.push_back(std::make_unique<Triangle>(vfC, vf5, vf6, floor_mat));

  // Ceiling
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc1, vc6, ceiling_mat));
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc2, vc1, ceiling_mat));
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc3, vc2, ceiling_mat));
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc4, vc3, ceiling_mat));
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc5, vc4, ceiling_mat));
  triangles_.push_back(std::make_unique<Triangle>(vcC, vc6, vc5, ceiling_mat));

  /* Counter-clockwise order, starting with front */

  // Wall1
  triangles_.push_back(std::make_unique<Triangle>(vf2, vc2, vf3, wall1_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf3, vc2, vc3, wall1_mat));

  // Wall 2
  triangles_.push_back(std::make_unique<Triangle>(vf3, vc3, vf4, wall2b_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf4, vc3, vc4, wall2a_mat));

  // Wall 3
  triangles_.push_back(std::make_unique<Triangle>(vf4, vc4, vf5, wall3_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf5, vc4, vc5, wall3_mat));

  // Wall 4
  triangles_.push_back(std::make_unique<Triangle>(vf5, vc5, vf6, wall4_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf6, vc5, vc6, wall4_mat));

  // Wall 5
  triangles_.push_back(std::make_unique<Triangle>(vf6, vc6, vf1, wall5_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf1, vc6, vc1, wall5_mat));

  // Wall 6
  triangles_.push_back(std::make_unique<Triangle>(vf1, vc1, vf2, wall6_mat));
  triangles_.push_back(std::make_unique<Triangle>(vf2, vc1, vc2, wall6_mat));
}

void Scene::InitLights() {
  scene_lights_.push_back(std::make_unique<PointLight>(Vertex(5.f,0.f,4.5f), 1.f, COLOR_WHITE));
  //scene_lights_.push_back(std::make_unique<PointLight>(Vertex(5.f,0.f,-3.5f), 100.f, COLOR_WHITE));

  //Create area light
  /*
  Vertex v1 = Vertex(5.f,-1.f, 4.99f);
  Vertex v2 = Vertex(7.f,-1.f, 4.99f);
  Vertex v3 = Vertex(7.f, 1.f, 4.99f);
  Vertex v4 = Vertex(5.f, 1.f, 4.99f);
  
  Material area_light_mat = Material(1.f, 0.f, 0.f, COLOR_WHITE, COLOR_WHITE);

  triangles_.push_back(std::make_unique<Triangle>(v1, v3, v2, area_light_mat));
  triangles_.push_back(std::make_unique<Triangle>(v1, v4, v3, area_light_mat));
  */
}
