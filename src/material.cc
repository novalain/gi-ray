#include "material.h"

Material::Material(float diffuse,
                   float specular,
                   float transparence,
                   ColorDbl color)
    : diffuse_(diffuse),
      specular_(specular),
      transparence_(transparence),
      color_(color) {}
