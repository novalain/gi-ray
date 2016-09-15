#ifndef RAY_H
#define RAY_H

#include "commons.h"
#include "triangle.h"

class Ray
{
private:
  Vertex* v_start_;
  Vertex* v_end_;

  Triangle* intersecting_triangle_;

public:
  Ray() {}
  Ray(Vertex* v1, Vertex* v2) : v_start_(v1), v_end_(v2) {}

  //TODO: implement GetTriangle() ?
  //TODO: implement GetStartVertex() ?
  //TODO: implement GetEndVertex() ?
};

#endif // RAY_H
