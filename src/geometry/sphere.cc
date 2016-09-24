#include "sphere.h"
#include "ray.h"
#include <iostream>

Sphere::Sphere(Vertex position, float radius, ColorDbl color)
    : SceneObject(position), color_(color) {
  assert(radius > 0);
  radius_ = radius;
}

bool Sphere::RayIntersection(Ray& ray, float& z) {
  Vertex o = ray.start();
  Vertex cp = position_;
  Direction l = glm::normalize(ray.end() - ray.start());
  float r = radius_;

  //float a = glm::dot(l, l); // == 1
  float b = glm::dot(2.f*l, o-cp);
  float c = glm::dot(o-cp, o-cp) - sqrt(r);

  float sqrt_term = pow(b/2, 2.f) - c;
  // Complex solution, no collision
  if (sqrt_term < 0) {
    return false;
  }

  Direction unit_normal = glm::normalize(ray.end() - this->position_);
  Vertex intersection_point = this->position_ + unit_normal * radius_;
  ray.set_intersection_point(new IntersectionPoint(intersection_point, unit_normal, Material(0.f, 0.f, 0.f, this->color_)));
  z = fmin(-b/2 + sqrt(sqrt_term), -b/2 - sqrt(sqrt_term));
  return true;
}
