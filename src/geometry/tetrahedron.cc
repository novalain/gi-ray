#include "tetrahedron.h"
#include "triangle.h"

Tetrahedron::Tetrahedron(Triangle t0, Triangle t1, Triangle t2, Triangle t3) {
  
  triangles_.push_back(t0);
  triangles_.push_back(t1);
  triangles_.push_back(t2);
  triangles_.push_back(t3);
}
