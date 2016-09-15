#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "commons.h"
#include <glm/glm.hpp>

class Triangle
{
private:
  Vertex v1_, v2_, v3_;

  glm::vec3 normal_;
  glm::vec3 color_;

  void CalcNormal();

public:
  Triangle() {}
  Triangle(Vertex v1, Vertex v2, Vertex v3);
  Triangle(Vertex v1, Vertex v2, Vertex v3, glm::vec3 color);

  glm::vec3 get_normal() { return normal_; }
  glm::vec3 get_color() { return color_; }

  //TODO: implement this
  // RayIntersection(Ray ray);

protected:
};

#endif // TRIANGLE_H
