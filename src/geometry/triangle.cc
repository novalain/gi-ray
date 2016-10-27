#include "triangle.h"
#include "intersection_point.h"
#include <iostream>

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2) : v0_(v0), v1_(v1), v2_(v2) {
  material_ = PERFECT_MIRROR;
  CalcNormal();
}

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2, glm::vec3 color) : v0_(v0), v1_(v1), v2_(v2) {
  material_ = Material(0,1,0,color, glm::vec3(0,0,0));
  CalcNormal();
}

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2, Material material) : v0_(v0), v1_(v1), v2_(v2), material_(material) {
  CalcNormal();
}

void Triangle::CalcNormal() {
  normal_ = glm::cross(v1_-v0_,v2_-v1_);
}

IntersectionPoint* Triangle::RayIntersection(Ray& ray) {
  Direction ps = ray.get_origin(); // eye_position
  Direction D = ray.get_direction();
  // Simple check if the triangle is facing the camera
  // If non-negative it is facing away from the camera, i.e. not visible from camera
    //if ( glm::dot(normal_,glm::normalize(D)) >= 0) {
    //  return FLT_MAX;
    //}

  // According to the Möller Trumbore intersection algorithm
  Direction T = ps - v0_;
  Direction E1 = v1_ - v0_;
  Direction E2 = v2_ - v0_;
  Direction P = glm::cross(D, E2);

  float det = glm::dot(P, E1);

  Direction Q = glm::cross(T, E1);
  float t = glm::dot(Q,E2) / det;
  float u = glm::dot(P, T) / det;
  float v = glm::dot(Q, D) / det;

  if(u >= 0 && v >= 0 && u+v <= 1 && t > 0.f) { //if collision with a triangle closer to cam than before
    Vertex intersection_vertex = (1-u-v)*v0_ + u*v1_ + v*v2_;
    // std::cout << "Successful update of pixel!" << std::endl; //TODO: remove when EVERYTHING is implemented
    return new IntersectionPoint(intersection_vertex, normal_, material_, t);
  }
  // std::cout << "Unsuccessful update of pixel! :'(" << std::endl; //TODO: remove when EVERYTHING is implemented
  return nullptr;
}

void Triangle::Print() const {
  std::cout << "v0 = (" << v0_.x << ", " << v0_.y << ", " << v0_.z << ",\n"
            << "v1 = (" << v1_.x << ", " << v1_.y << ", " << v1_.z << ",\n"
            << "v2 = (" << v2_.x << ", " << v2_.y << ", " << v2_.z << ",\n" << std::endl;
}
