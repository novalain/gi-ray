#include "triangle.h"
#include <iostream>

Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3) : v1_(v1), v2_(v2), v3_(v3) {
    color_ = COLOR_WHITE;
    CalcNormal();
}

Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3, glm::vec3 color) : v1_(v1), v2_(v2), v3_(v3), color_(color) {
    CalcNormal();
}

void Triangle::CalcNormal() {
    normal_ = glm::cross(v2_-v1_,v3_-v2_);
}
