#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "commons.h"
#include "scene_object.h"

class PointLight : public SceneObject {
protected:
private:
  Vertex position_;
  float intensity_;
  ColorDbl color_;
public:
  PointLight();
  PointLight(Vertex position);
  PointLight(Vertex position, float intensity);
  PointLight(Vertex position, float intensity, ColorDbl color);
  virtual bool RayIntersection(Ray& ray, float& z) {
    return false; // a ray cannot hit a point of zero area
  }

  Vertex get_position() { return color_; }
  float get_intensity() { return intensity_; }
  ColorDbl get_color() { return color_; }
};

#endif //POINT_LIGHT_H
