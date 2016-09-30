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
  position_ = position;
  intensity_ = intensity;
  color_ = color;
}
