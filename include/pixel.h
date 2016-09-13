#ifndef PIXEL_H
#define PIXEL_H

#include <glm::glm.hpp>
#include "ray.h"

class Pixel
{
private:
  glm::vec3 color_;

  //TODO: extend this to allow more than one ray
  Ray* ray_;

public:
  Pixel() {}
  Pixel(Vertex* v1, Vertex* v2) : v_start_(v1), v_end_(v2) {}

  Ray get_ray() { return ray_; }
};

#endif // PIXEL_H
