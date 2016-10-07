#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "triangle.h"
#include <vector>

class Tetrahedron {
private:
  std::vector<Triangle> triangles_;
public:
  Tetrahedron(Triangle& t0, Triangle& t1, Triangle& t2, Triangle& t4);
  Tetrahedron(float width, float height, Vertex position, Material material);
};

#endif // TETRAHEDRON_H
