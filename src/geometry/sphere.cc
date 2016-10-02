#include "sphere.h"
#include "ray.h"
#include <iostream>

Sphere::Sphere(Vertex position, float radius, ColorDbl color)
    : SceneObject(position) {
  assert(radius > 0);
  radius_ = radius;
  material_ = Material(0,1,0,color);
}

Sphere::Sphere(Vertex position, float radius, Material material)
    : SceneObject(position), material_(material) {
  assert(radius > 0);
  radius_ = radius;
}

bool Sphere::RayIntersection(Ray& ray, float& z) {
  Vertex o = ray.get_origin();
  Vertex cp = position_;
  Direction sphere_to_ray = o-cp;
  Direction l = glm::normalize(ray.get_direction());
  float r = radius_;

  //float a = glm::dot(l, l); // == 1
  float b = glm::dot(2.f*l, sphere_to_ray);
  float c = glm::dot(sphere_to_ray, sphere_to_ray) - sqrt(r);

  float sqrt_term = pow(b/2, 2.f) - c;
  // Complex solution, no collision
  if (sqrt_term < 0) {
    return false;
  }

  Direction unit_normal = -l;
  Vertex intersection_point = position_ + unit_normal * radius_;
  ray.set_intersection_point(new IntersectionPoint(intersection_point, unit_normal, material_));
  z = fmin(-b/2 + sqrt(sqrt_term), -b/2 - sqrt(sqrt_term));
  return true;
}
