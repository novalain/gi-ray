#include "triangle.h"
#include <iostream>

#define ps ray->start()
#define pe ray->end()

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2) : v0_(v0), v1_(v1), v2_(v2) {
  color_ = COLOR_WHITE;
  CalcNormal();
}

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2, glm::vec3 color) : v0_(v0), v1_(v1), v2_(v2), color_(color) {
  CalcNormal();
}

void Triangle::CalcNormal() {
  normal_ = glm::cross(v1_-v0_,v2_-v1_);
}

bool Triangle::RayIntersection(Ray* ray) {
  Vertex T = ps - v0_;
  Direction E1 = v1_ - v0_;
  Direction E2 = v2_ - v0_;
  Direction D = pe - ps;
  Direction P = glm::cross(D, E2);
  Direction Q = glm::cross(T, E1);
  float t = (glm::dot(Q, E2) / glm::dot(P, E1));
  float u = (glm::dot(P, T) / glm::dot(P, E1));
  float v = (glm::dot(Q, D) / glm::dot(P, E1));

  // std::cout << t << "," << u << "," << v << std::endl;
  if(u >= 0 && v >= 0 && u+v <= 1) {
    Vertex intersection_point = (1-u-v)*v0_ +u*v1_ + v*v2_;
    ray->set_intersecting_triangle(this);
    ray->set_color(this->color_);
    std::cout << "Successful update of pixel!" << std::endl;
    return true;
  } else {
    // std::cout << "Unsuccessful update of pixel! :'(" << std::endl;
    return false;
  }
}
