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

IntersectionPoint* Sphere::RayIntersection(Ray& ray) {
  Direction L = ray.get_origin() - position_;
  Direction dir = ray.get_direction();
  float radius2 = radius_ * radius_;
  float a = glm::dot(dir, dir);
  float b = 2 * glm::dot(dir, L);
  float c = glm::dot(L, L) - radius2;
  float t0, t1;
  if (!SolveQuadratic(a, b, c, t0, t1)) {
    return nullptr;
  }
  if (t0 < 0) {
    if (t1 < 0) {
      return nullptr;
    }
    t0 = t1;
  }
  Vertex intersection_point = ray.get_origin() + ray.get_direction() * t0;
  Direction normal = intersection_point - position_;
  return new IntersectionPoint(intersection_point, normal, material_, t0);
}

bool Sphere::SolveQuadratic(const float& a,
                            const float& b,
                            const float& c,
                            float& x0,
                            float& x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0) {
    return false;
  } else if (discr == 0) {
    x0 = x1 = -0.5 * b / a;
  } else {
    float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1) {
    std::swap(x0, x1);
  }
  return true;
}
