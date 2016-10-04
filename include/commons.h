#ifndef COMMONS_H
#define COMMONS_H

#include <glm/glm.hpp>

typedef glm::vec3 Vertex;
typedef glm::vec3 Direction;
typedef glm::vec3 ColorDbl;

#define COLOR_WHITE   ColorDbl(255,255,255)
#define COLOR_BLACK   ColorDbl(  0,  0,  0)
#define COLOR_RED     ColorDbl(255,  0,  0)
#define COLOR_GREEN   ColorDbl(  0,255,  0)
#define COLOR_BLUE    ColorDbl(  0,  0,255)
#define COLOR_CYAN    ColorDbl(  0,255,255)
#define COLOR_MAGENTA ColorDbl(255,  0,255)
#define COLOR_YELLOW  ColorDbl(255,255,  0)
#define COLOR_PINK    ColorDbl(235, 16,255)

#define PERFECT_MIRROR Material(0,1.f,0,COLOR_WHITE)
#define GLASS_MAT Material(0,0,1.0f,COLOR_WHITE)

const float REFRACTION_INDEX_AIR = 1.f;
const float REFRACTION_INDEX_GLASS = 1.5f;

#endif //COMMONS_H
