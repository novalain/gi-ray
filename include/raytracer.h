#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include "ray.h"
#include "commons.h"
#include <memory>
#include "intersection_point.h"

class Scene;

class Raytracer {
private:
  ColorDbl HandleRefraction(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth);
  ColorDbl Shade(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth);
  ColorDbl CalculateDirectIllumination(Ray& ray, IntersectionPoint& p, Scene& scene);
  std::unique_ptr<IntersectionPoint> GetClosestIntersectionPoint(Ray& ray, Scene& scene);
  bool CastShadowRay(Ray& ray, Scene& scene, Direction& light_direction);
 
public:
  Raytracer();
  ColorDbl Raytrace(Ray& ray, Scene& scene, unsigned int depth);
};

#endif // Raytracer_H
