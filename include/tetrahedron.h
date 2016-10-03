#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "triangle_based_object.h"
#include "triangle.h"

class Tetrahedron : public TriangleBasedObject {
public:
  Tetrahedron(Triangle& t0, Triangle& t1, Triangle& t2, Triangle& t4);
  Tetrahedron(float width, float height, Vertex position, Material material);
};

#endif // TETRAHEDRON_H
