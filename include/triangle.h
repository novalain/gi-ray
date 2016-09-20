#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "commons.h"
#include "ray.h"

class Triangle
{
private:
  Vertex v0_, v1_, v2_;

  glm::vec3 normal_;
  glm::vec3 color_;

  void CalcNormal();
public:
  Triangle() {}
  Triangle(Vertex v0, Vertex v1, Vertex v2);
  Triangle(Vertex v0, Vertex v1, Vertex v2, glm::vec3 color);

  glm::vec3 get_normal() { return normal_; }
  glm::vec3 get_color() { return color_; }

  bool RayIntersection(Ray& ray, float& z);

  void Print() const;
};

#endif // TRIANGLE_H
