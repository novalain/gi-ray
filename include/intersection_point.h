#ifndef INTERSECTION_POINT_H
#define INTERSECTION_POINT_H

#include "commons.h"
#include "material.h"

class IntersectionPoint {
private:
  Vertex position_;
  Direction normal_;
  Material material_;
public:
  IntersectionPoint();
  IntersectionPoint(Vertex position, Direction normal, Material material_);

  Vertex get_position() { return position_; }
  Direction get_normal() { return normal_; }
  Material get_material() { return material_; }
};

#endif //INTERSECTION_POINT_H
