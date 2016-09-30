#ifndef PIXEL_H
#define PIXEL_H

#include <glm/glm.hpp>

class Ray;

class Pixel {
private:
  glm::vec3 color_;

  //TODO: extend this to allow more than one ray
  Ray* ray_;

public:
  Pixel() {}

  glm::vec3 get_color() { return color_; }
  Ray* get_ray_pointer() { return ray_; }

  void set_color(glm::vec3 color) { color_ = color; }
};

#endif // PIXEL_H
