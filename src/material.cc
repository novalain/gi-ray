#include "material.h"

Material::Material(float diffuse,
                   float specular,
                   float transparence,
                   ColorDbl color)
    : diffuse_(diffuse),
      specular_(specular),
      transparence_(transparence),
      color_(color) {
  float sum = diffuse_ + specular_ + transparence_;
  assert( sum <= 1.0f );
}
