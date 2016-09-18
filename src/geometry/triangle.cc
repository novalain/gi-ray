#include "triangle.h"
#include <iostream> //TODO: remove when EVERYTHING is implemented

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

float Triangle::RayIntersection(Ray& ray) {
  Direction D = ray.end() - ray.start(); // vector from eye to pixel
  // Simple check if the triangle is facing the camera
  // If non-negative it is facing away from the camera, i.e. not visible from camera
  if( glm::dot(normal_,glm::normalize(D)) >= 0) {
    return FLT_MAX;
  }

  // According to the Möller Trumbore intersection algorithm
  Direction ps = ray.start(); // eye_position
  Direction pe = ray.end(); // pixel centre
  Direction T = ray.start() - v0_;
  Direction E1 = v1_ - v0_;
  Direction E2 = v2_ - v0_;
  Direction P = glm::cross(D, E2);
  Direction Q = glm::cross(T, E1);
  float t = (glm::dot(Q,E2) / glm::dot(P, E1));
  float u = (glm::dot(P, T) / glm::dot(P, E1));
  float v = (glm::dot(Q, D) / glm::dot(P, E1));

  if(u >= 0 && v >= 0 && u+v <= 1) { //if collision with triangle
    Vertex intersection_point = (1-u-v)*v0_ + u*v1_ + v*v2_;
    ray.set_intersecting_triangle(this);
    ray.set_color(this->color_);
    // std::cout << "Successful update of pixel!" << std::endl; //TODO: remove when EVERYTHING is implemented
    return glm::distance(intersection_point, pe); //return distance of collision point to pixel_centre
  } else {
    // std::cout << "Unsuccessful update of pixel! :'(" << std::endl; //TODO: remove when EVERYTHING is implemented
    return FLT_MAX;
  }
}
