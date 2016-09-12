#include "triangle.h"
#include <iostream>

// TODO: Decide which way is best! Here or in .h file????
// Triangle::Triangle() {}

// Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3) {
//     v1_ = v1;
//     v2_ = v2;
//     v3_ = v3;
// }

void Triangle::Print() {
    v1_.Print();
    v2_.Print();
    v3_.Print();
}
