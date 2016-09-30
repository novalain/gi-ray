#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "commons.h"
#include "light.h"

class PointLight : public Light {
protected:
public:
  PointLight();
  PointLight(Vertex position);
  PointLight(Vertex position, float intensity);
  PointLight(Vertex position, float intensity, ColorDbl color);

  virtual bool RayIntersection(Ray& ray, float& z) {
    return false; // a ray cannot hit a point of zero area
  }
};

#endif //POINT_LIGHT_H
