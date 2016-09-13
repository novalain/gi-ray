#ifndef PIXEL_H
#define PIXEL_H

#include <glm/glm.hpp>
#include "ray.h"

class Pixel
{
private:
  glm::vec3 color_;

  //TODO: extend this to allow more than one ray
  Ray* ray_;

public:
  Pixel() {}

  Ray* get_ray_pointer() { return ray_; }
};

#endif // PIXEL_H
