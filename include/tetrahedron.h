#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "triangle_based_object.h"
#include "triangle.h"

class Tetrahedron : TriangleBasedObject {
  public:
    Tetrahedron(Triangle t0, Triangle t1, Triangle t2, Triangle t4);
};

#endif // TETRAHEDRON_H
