#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "commons.h"
#include "ray.h"
#include "material.h"

class Triangle {
private:
  Vertex v0_, v1_, v2_;

  Direction normal_;
  Material material_;

  void CalcNormal();
public:
  Triangle() {}
  Triangle(Vertex v0, Vertex v1, Vertex v2);
  Triangle(Vertex v0, Vertex v1, Vertex v2, ColorDbl color);
  Triangle(Vertex v0, Vertex v1, Vertex v2, Material material);

  Direction get_normal() { return normal_; }
  Material get_material() { return material_; }

  bool RayIntersection(Ray& ray, float& z);

  void Print() const;
};

#endif // TRIANGLE_H
