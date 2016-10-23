#include "point_light.h"

PointLight::PointLight() {
  position_ = Vertex(0,0,0);
  intensity_ = 1000; // TODO: some default value. Change depending on how it turns out?
  color_ = COLOR_WHITE; // White light
}

PointLight::PointLight(Vertex position) {
  position_ = position;
  intensity_ = 1000; // TODO: some default value. Change depending on how it turns out?
  color_ = COLOR_WHITE;
}

PointLight::PointLight(Vertex position, float intensity) {
  position_ = position;
  intensity_ = intensity;
  color_ = COLOR_WHITE;
}

PointLight::PointLight(Vertex position, float intensity, ColorDbl color) {
  assert(color.x <= 1.f && color.x >= 0.f && color.y <= 1.f && color.y >= 0.f &&
         color.z <= 1.f && color.z >= 0.f && intensity <= 1.f &&
         intensity >= 0.f);
  position_ = position;
  intensity_ = intensity;
  color_ = color;
}
