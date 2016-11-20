#ifndef COMMONS_H
#define COMMONS_H

#include <glm/glm.hpp>

typedef glm::vec3 Vertex;
typedef glm::vec3 Direction;
typedef glm::vec3 ColorDbl;

#define COLOR_WHITE   ColorDbl(255.f,255.f,255.f)/255.f
#define COLOR_BLACK   ColorDbl(  0.f,  0.f,  0.f)/255.f
#define COLOR_RED     ColorDbl(255.f,  0.f,  0.f)/255.f
#define COLOR_GREEN   ColorDbl(  0.f,255.f,  0.f)/255.f
#define COLOR_BLUE    ColorDbl(  0.f,  0.f,255.f)/255.f
#define COLOR_CYAN    ColorDbl(  0.f,255.f,255.f)/255.f
#define COLOR_MAGENTA ColorDbl(255.f,  0.f,255.f)/255.f
#define COLOR_YELLOW  ColorDbl(255.f,255.f,  0.f)/255.f
#define COLOR_PINK    ColorDbl(235.f, 16.f,255.f)/255.f

#define PERFECT_MIRROR Material(0,1.f,0,COLOR_WHITE, glm::vec3(0,0,0))
#define GLASS_MAT Material(0.1f,0.3f,0.6f,COLOR_RED, glm::vec3(0,0,0))

const float REFRACTION_INDEX_AIR = 1.f;
const float REFRACTION_INDEX_GLASS = 1.5f;

#endif //COMMONS_H
