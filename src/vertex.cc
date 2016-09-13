#include "vertex.h"
#include <iostream> //TODO: remove this and the Print() function? just for simple debug?

Vertex::Vertex() {
  coordinates_ = glm::vec4(0,0,0,1);
}

Vertex::Vertex(double x, double y, double z, double w) {
  coordinates_ = glm::vec4(x,y,z,w);
}

Vertex::Vertex(glm::vec3 pos){
  coordinates_ = glm::vec4(pos,1);
}

void Vertex::Print() {
  std::cout << "x = " << coordinates_.x << ", y = " << coordinates_.y << ", z = " << coordinates_.z << ", w = " << coordinates_.w << std::endl;
}
