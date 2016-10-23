#ifndef COMMONS_H
#define COMMONS_H

#include <glm/glm.hpp>
#include <iostream>

// TODO: Should make a namespace for this file

typedef glm::vec3 Vertex;
typedef glm::vec3 Direction;
typedef glm::vec3 ColorDbl;

#define COLOR_WHITE   ColorDbl(1.f, 1.f, 1.f)
#define COLOR_BLACK   ColorDbl(0.f, 0.f, 0.f)
#define COLOR_RED     ColorDbl(1.f, 0.f, 0.f)
#define COLOR_GREEN   ColorDbl(0.f, 1.f, 0.f)
#define COLOR_BLUE    ColorDbl(0.f, 0.f, 1.f)
#define COLOR_CYAN    ColorDbl(0.f, 1.f, 1.f)
#define COLOR_MAGENTA ColorDbl(1.f, 0.f, 1.f)
#define COLOR_YELLOW  ColorDbl(1.f, 1.f, 0.f)
#define COLOR_PINK    ColorDbl(235.f, 16.f ,255.f) / 255.f

#define PERFECT_MIRROR Material(0,1.f,0,COLOR_WHITE)
#define GLASS_MAT Material(0,0,1.0f,COLOR_WHITE)

const float REFRACTION_INDEX_AIR = 1.f;
const float REFRACTION_INDEX_GLASS = 1.5f;

inline float clamp(const float& lo, const float& hi, const float& v) {
  return std::max(lo, std::min(hi, v));
}

#endif //COMMONS_H
