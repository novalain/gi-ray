#ifndef LIGHT_H
#define LIGHT_H

#include "scene_object.h"

class Light : public SceneObject {
private:
protected:
  Vertex position_;
  float intensity_;
  ColorDbl color_;
public:
  Light() = default;
  ~Light() {}
  float get_intensity() { return intensity_; }
  ColorDbl get_color() { return color_; }
};

#endif // LIGHT_H
