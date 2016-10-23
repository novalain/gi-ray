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
  assert(sum <= 1.0f && sum >= 0.f && color.x <= 1.f && color.x >= 0.f &&
         color.y <= 1.f && color.y >= 0.f && color.z <= 1.f && color.z >= 0.f);
}
