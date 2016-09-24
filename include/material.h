#ifndef MATERIAL_H
#define MATERIAL_H

#include "commons.h"

class Material {
private:
  float diffuse_;
  float specular_;
  float transparence_;
  ColorDbl color_;

  float get_diffuse() { return diffuse_; }
  float get_specular() { return specular_; }
  float get_transparence() { return transparence_; }

public:
  Material(float diffuse, float specular, float transparence, ColorDbl color);
};

#endif // MATERIAL_H
