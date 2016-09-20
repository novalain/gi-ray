#include "sphere.h"
#include "ray.h"
#include <iostream>

Sphere::Sphere(Vertex position, float radius, ColorDbl color)
    : SceneObject(position), radius_(radius), color_(color) {}

bool Sphere::RayIntersection(Ray& ray, float& z) {
  return false;
}
