#include "tetrahedron.h"
#include "triangle.h"
#include <cmath>
#include <iostream>

Tetrahedron::Tetrahedron(Triangle& t0,
                         Triangle& t1,
                         Triangle& t2,
                         Triangle& t3) {
  triangles_.push_back(t0);
  triangles_.push_back(t1);
  triangles_.push_back(t2);
  triangles_.push_back(t3);
}

Tetrahedron::Tetrahedron(float width, float height, Vertex position) {
  Vertex v0 = Vertex(width,0,0);
  float a = 2*M_PI/3;
  glm::mat3 rotmat = glm::mat3(cos(a), sin(a), 0,
                              -sin(a), cos(a), 0,
                                  0 ,      0 , 1);
  Vertex v1 = rotmat*v0;
  Vertex v2 = rotmat*v1;
  Vertex v3 = Vertex(0,0,height);
  //TODO This should be nicer.. positioning the vertexes
  v0 = v0+position;
  v1 = v1+position;
  v2 = v2+position;
  v3 = v3+position;

  //TODO the colours should not be like this later on
  Triangle t0 = Triangle(v0, v2, v1, COLOR_PINK); //bottom
  Triangle t1 = Triangle(v1, v2, v3, COLOR_RED); // "front"
  Triangle t2 = Triangle(v0, v1, v3, COLOR_YELLOW); // "left back"
  Triangle t3 = Triangle(v0, v3, v2, COLOR_CYAN); // "right back"
  triangles_.push_back(t0);
  triangles_.push_back(t1);
  triangles_.push_back(t2);
  triangles_.push_back(t3);
}
