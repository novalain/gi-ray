#ifndef MATERIAL_H
#define MATERIAL_H

#include "commons.h"

class Material {
private:
  float diffuse_;
  float specular_;
  float transparence_;
  ColorDbl color_;

public:
  Material(float diffuse, float specular, float transparence, ColorDbl color);

  float get_diffuse() { return diffuse_; }
  float get_specular() { return specular_; }
  float get_transparence() { return transparence_; }
  ColorDbl get_color() { return color_; }
};

#endif // MATERIAL_H
