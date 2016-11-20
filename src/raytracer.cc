#define _USE_MATH_DEFINES // Needed to run in windows/visual studio
#include "raytracer.h"
#include "scene.h"
// TODO: Remove when we have all point lights in vector
#include "point_light.h"
#include <random>
#include <iostream>

// TODO: Place these somewhere that makes the most sense and remove some?
const float EPSILON = 0.00001f;
const float REFRACTION_FACTOR_OI = REFRACTION_INDEX_AIR / REFRACTION_INDEX_GLASS; // outside->in
const float REFRACTION_FACTOR_IO = REFRACTION_INDEX_GLASS / REFRACTION_INDEX_AIR; // inside->out
const float CRITICAL_ANGLE = asin(REFRACTION_FACTOR_OI);
const unsigned int MAX_DEPTH = 10; // What Max depth makes sense?
const float PI2 =(float) M_PI * 2.f;

std::default_random_engine generator2;
std::uniform_real_distribution<float> distribution2(0, 1);

Raytracer::Raytracer() {}

ColorDbl Raytracer::CalculateDirectIllumination(Ray& ray, IntersectionPoint& p, Scene& scene) {
  const std::vector<std::unique_ptr<Light>>& lights = scene.get_lights();
  ColorDbl color_accumulator = COLOR_BLACK;
  for (auto& light : lights) {
    Direction light_direction = light->get_position() - p.get_position();

    // Set dot product to zero if light is behind the surface
    Direction unit_surface_normal = glm::normalize(p.get_normal());
    Vertex shadow_point_origin = p.get_position() + unit_surface_normal * 0.00001f;

    // Compute shadow ray
    Ray shadow_ray = Ray(shadow_point_origin, light_direction);
    bool in_shadow = CastShadowRay(shadow_ray, scene, light_direction);

    if (!in_shadow) {
      Direction unit_light_direction = glm::normalize(light_direction);
      float l_dot_n = fmax(0.f, glm::dot(unit_light_direction, unit_surface_normal));
      color_accumulator += light->get_intensity() * light->get_color() * l_dot_n;
    }
  }
  return color_accumulator * p.get_material().get_color();
}

ColorDbl Raytracer::HandleDiffuse(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth) {

  //STOP ON SECOND DIFFUSE HIT
  if (ray.has_hit_diffuse) {
    return CalculateDirectIllumination(ray, p, scene);
  }
  ray.has_hit_diffuse = true;
  Direction w = glm::normalize(p.get_normal()); // TODO: this should not have to be normalized here, it should already be normalized!!
  // Generalized formula for finding tangent u given a unit length normal vector w
  Direction u = glm::normalize(glm::cross(fabs(w.x) > .1f ? Direction(0.f, 1.f, 0.f) : Direction(1.f, 0.f, 0.f), w));
  Direction v = glm::cross(w, u);

  // Random direction d within hemisphere
  // Random angle
  float r1 = PI2 * distribution2(generator2);
  // Random distance from center
  float r2 = distribution2(generator2);
  float r2s = sqrtf(r2);
  Direction d = glm::normalize(u * (float)cos(r1) * r2s + v*(float)sin(r1) * r2s + w * sqrtf(1 - r2));

  Vertex reflection_point_origin = p.get_position() + w * 0.00001f;
  Ray new_ray = Ray(reflection_point_origin, d, 0.2f * p.get_material().get_diffuse());
  new_ray.has_hit_diffuse = ray.has_hit_diffuse;

  //TODO why are we multiplying here? Shouldn't it be addition?
  return CalculateDirectIllumination(ray, p, scene) * Raytrace(new_ray, scene, depth + 1);
}


ColorDbl Raytracer::HandleSpecular(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth) {
  Direction n = glm::normalize(p.get_normal());
  Direction d = ray.get_direction();

  Vertex reflection_point_origin = p.get_position() + n * 0.00001f;
  Direction reflection_direction = d - 2 * (glm::dot(d, n))*n;
  Ray reflection_ray = Ray(reflection_point_origin, reflection_direction);
  reflection_ray.has_hit_diffuse = ray.has_hit_diffuse;
  return Raytrace(reflection_ray, scene, depth + 1);
}

// TODO: Refactor later
// Done - according to lecture4 slides
ColorDbl Raytracer::HandleRefraction(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth) {
  //TODO: check why these normalizations are NOT redundant
  Direction n = glm::normalize(p.get_normal());
  Direction I = ray.get_direction();
  // assert(std::abs(1.0f - (float)glm::length(p.get_normal())) < EPSILON);
  assert(std::abs(1.0f - (float) glm::length(I)) < EPSILON);
  float I_dot_n = glm::dot(n,I);
  bool is_inside_object = ray.get_refraction_status();

  if ( !is_inside_object ) {
    Direction T = REFRACTION_FACTOR_OI * I - n*(REFRACTION_FACTOR_OI*I_dot_n +
        sqrtf(1 - REFRACTION_FACTOR_OI*REFRACTION_FACTOR_OI * (1 - I_dot_n*I_dot_n)));
    Vertex refraction_point_origin = p.get_position() - n * EPSILON;
    Ray refraction_ray = Ray(refraction_point_origin, T);
    refraction_ray.has_hit_diffuse = ray.has_hit_diffuse;
    refraction_ray.set_refraction_status(true);
    return p.get_material().get_transparence() * Raytrace(refraction_ray, scene, depth + 1);
  } else { // we are inside of a glass object, trying to go outside
    n = -n; // because we are at the inside of the object now
    // calculate angle between normal and incoming ray direction
    // according to formula: dot(u,v) = |u||v|cos(a) = cos(a), if |u|=|v|=1
    // => a = arccos(dot(u,v)), providing u and v are normalized
    float alpha = acos(I_dot_n);

    if ( alpha > CRITICAL_ANGLE ) { // if total inner reflection
      Vertex inner_reflection_ray_origin = p.get_position() + n * EPSILON;
      Direction reflection_direction = I - 2.f*(glm::dot(I, n))*n;
      Ray total_inner_reflection_ray = Ray(inner_reflection_ray_origin, reflection_direction);
      total_inner_reflection_ray.has_hit_diffuse = ray.has_hit_diffuse;
      total_inner_reflection_ray.set_refraction_status(true);
      return p.get_material().get_transparence() * Raytrace(total_inner_reflection_ray, scene, depth + 1);
    } else if ( CRITICAL_ANGLE == alpha ) {
      std::cerr << "THIS SHOULD NEVER (or at least very rarely) BE PRINTED!!!!!" << std::endl;
      return COLOR_BLACK;
    }
    // else exiting glass object!
    Direction T = REFRACTION_FACTOR_IO * I - n*(-REFRACTION_FACTOR_IO*I_dot_n +
        sqrtf(1.f - REFRACTION_FACTOR_IO*REFRACTION_FACTOR_IO * (1.f - I_dot_n*I_dot_n)));
    // TODO: this can probably be removed, but might be annoying to debug if wrong
    if ( 1.0f - glm::length(T) > EPSILON ) {
      std::cerr << "THIS SHOULD NOT BE PRINTED! Length of T = " << glm::length(T) << std::endl;
    }
    Vertex outgoing_refraction_ray_origin = p.get_position() - n * EPSILON;
    Ray refraction_ray = Ray(outgoing_refraction_ray_origin, T);
    refraction_ray.has_hit_diffuse = ray.has_hit_diffuse;
    refraction_ray.set_refraction_status(false);
    return p.get_material().get_transparence() * Raytrace(refraction_ray, scene, depth + 1);
  }
}

std::unique_ptr<IntersectionPoint> Raytracer::GetClosestIntersectionPoint(Ray& ray, Scene& scene) {
  const std::vector<std::unique_ptr<Triangle>>& triangles = scene.get_triangles();
  const std::vector<std::unique_ptr<Sphere>>& spheres = scene.get_spheres();
  std::unique_ptr<IntersectionPoint> return_point;

  //To make sure we update the z_buffer upon collision.
  float z_buffer = FLT_MAX;
  for (auto& object : triangles) {
    std::unique_ptr<IntersectionPoint> p = object->RayIntersection(ray);
    if (p && p->get_z() < z_buffer) {
      z_buffer = p->get_z();
      return_point = std::move(p);
    }
  }
  for (auto& object : spheres) {
    std::unique_ptr<IntersectionPoint> p = object->RayIntersection(ray);
    if (p && p->get_z() < z_buffer) {
      z_buffer = p->get_z();
      return_point = std::move(p);
    }
  }
  return return_point;
}

bool Raytracer::CastShadowRay(Ray& ray, Scene& scene, Direction& light_direction) {
  const std::vector<std::unique_ptr<Triangle>>& triangles = scene.get_triangles();
  const std::vector<std::unique_ptr<Sphere>>& spheres = scene.get_spheres();
  float z_buffer = FLT_MAX;
  for (auto& object : triangles) {
    std::unique_ptr<IntersectionPoint> p = object->RayIntersection(ray);
    if (p && p->get_z() < glm::length(light_direction) &&
        p->get_material().get_transparence() == 0.f) {
      return true;
    }
  }
  for (auto& object : spheres) {
    std::unique_ptr<IntersectionPoint> p = object->RayIntersection(ray);
    if (p && p->get_z() < glm::length(light_direction) &&
      p->get_material().get_transparence() == 0.f) {
      return true;
    }
  }
  return false;
}

ColorDbl Raytracer::Raytrace(Ray& ray, Scene& scene, unsigned int depth) {
  float z_buffer = FLT_MAX;
  std::unique_ptr<IntersectionPoint> p = GetClosestIntersectionPoint(ray, scene);
  if (p) {
    if (depth > MAX_DEPTH) {
      return CalculateDirectIllumination(ray, *p, scene);
    }
    if (p->get_material().get_specular() > 0.f) {
      return HandleSpecular(ray, *p, scene, depth);
    }
    if (p->get_material().get_transparence() > 0.f) { // If object has refractive component
      return p->get_material().get_color() * HandleRefraction(ray, *p, scene, depth);
    }
    if (p->get_material().get_diffuse() > 0.f) {
      return HandleDiffuse(ray, *p, scene, depth);
    }
    //return ray.importance * (intersection_point->get_material().get_emission() + Shade(ray, *intersection_point, scene, depth));
  }
  std::cerr << "\nLigg här och gnag... " << std::endl;
  return COLOR_BLACK;
}