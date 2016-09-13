#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include <glm/glm.hpp>

class Triangle
{
private:
  Vertex v1_, v2_, v3_;

  glm::vec3 normal_;
  glm::vec3 color_;

public:
  Triangle() {}
  Triangle(Vertex v1, Vertex v2, Vertex v3) : v1_(v1), v2_(v2), v3_(v3) {}

  glm::vec3 get_normal() { return normal_; }
  glm::vec3 get_color() { return color_; }

  void Print();

  //TODO: implement this
  // RayIntersection(Ray ray);

protected:
};

#endif // TRIANGLE_H
