#ifndef MATERIAL_H
#define MATERIAL_H

#include "commons.h"

class Material {
private:
  float diffuse_;
  float specular_;
  float transparence_;
  glm::vec3 emission_;
  ColorDbl color_;

public:
  Material() = default;
  Material(float diffuse, float specular, float transparence, ColorDbl color, glm::vec3 emission);

  float get_diffuse() { return diffuse_; }
  glm::vec3 get_emission() { return emission_; }
  float get_specular() { return specular_; }
  float get_transparence() { return transparence_; }
  ColorDbl get_color() { return color_; }
};

#endif // MATERIAL_H
