#ifndef RAY_H
#define RAY_H

#include "commons.h"
// TODO: Can't forward declare due to destructor, make smart pointer
#include "intersection_point.h"

class Ray {
private:
  Vertex origin_;
  Direction direction_;
  // TODO: make smart pointer and get rid of destructor
  IntersectionPoint* intersection_point_ = nullptr;

public:
  Ray() {}
  ~Ray() { if(intersection_point_) { delete intersection_point_; } }
  Ray(Vertex origin, Direction direction) : origin_(origin), direction_(glm::normalize(direction)) {}

  Vertex get_origin() { return origin_; }
  Direction get_direction() { return direction_; }

  void set_intersection_point(IntersectionPoint* p) { intersection_point_ = p; }
  IntersectionPoint* get_intersection_point() { return intersection_point_; }

};

#endif // RAY_H
