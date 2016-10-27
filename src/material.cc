#include "material.h"

Material::Material(float diffuse,
                   float specular,
                   float transparence,
                   ColorDbl color,
                   glm::vec3 emission)
    : diffuse_(diffuse),
      specular_(specular),
      transparence_(transparence),
      color_(color),
      emission_(emission) {
  float sum = diffuse_ + specular_ + transparence_;
  assert( sum <= 1.0f );
}
