#ifndef LIGHT_H
#define LIGHT_H

#include "commons.h"

class Light {
private:
protected:
  float intensity_;
  Vertex position_;
  ColorDbl color_;
public:
  virtual ~Light() = default;
  float get_intensity() { return intensity_; }
  Vertex get_position() { return position_; }
  ColorDbl get_color() { return color_; }
};

#endif // LIGHT_H
