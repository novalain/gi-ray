#ifndef RAY_H
#define RAY_H

#include "commons.h"
// TODO: Can't forward declare due to destructorm, make smart pointer
#include "intersection_point.h"

class Ray {
private:
  Vertex v_start_;
  Vertex v_end_;
  // TODO: make smart pointer
  IntersectionPoint* intersection_point_ = nullptr;

public:
  Ray() {}
  ~Ray() { if(intersection_point_) { delete intersection_point_; } }
  Ray(Vertex v_start, Vertex v_end) : v_start_(v_start), v_end_(v_end) {}

  //TODO: implement GetTriangle() ?
  Vertex start() { return v_start_; }
  Vertex end() { return v_end_; }

  void set_intersection_point(IntersectionPoint* p) { intersection_point_ = p; }
  IntersectionPoint* get_intersection_point() { return intersection_point_; }

};

#endif // RAY_H
