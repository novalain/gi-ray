#ifndef RAY_H
#define RAY_H

#include "commons.h"
// TODO: Can't forward declare due to destructor, make smart pointer
#include "intersection_point.h"

class Ray {
private:
  Vertex origin_;
  Direction direction_;
  float importance_; // To be used in Monte-Carlo
  bool is_inside_object = false; // if inside an object
  // TODO: make smart pointer and get rid of destructor
  IntersectionPoint* intersection_point_ = nullptr;

public:
  Ray() {}
  ~Ray() { if(intersection_point_) { delete intersection_point_; } }
  Ray(Vertex origin, Direction direction) : origin_(origin),
      direction_(glm::normalize(direction)), importance_(1.0f) {}
  Ray(Vertex origin, Direction direction, float importance) : origin_(origin),
      direction_(glm::normalize(direction)), importance_(importance) {}

  // maybe change name to, is_inside_object()?
  bool get_refraction_status() { return is_inside_object; }
  float get_importance() { return importance_; }
  Vertex get_origin() { return origin_; }
  Direction get_direction() { return direction_; }
  IntersectionPoint* get_intersection_point() { return intersection_point_; }

  void set_intersection_point(IntersectionPoint* p) { intersection_point_ = p; }
  void set_refraction_status(bool status) { is_inside_object = status; }
};

#endif // RAY_H
