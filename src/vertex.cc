#include "vertex.h"
#include <iostream>

Vertex::Vertex() {
    std::cout << "Default Vertex Constructor called!" << std::endl;
    coordinates = glm::vec4(0,0,0,1);
}

Vertex::Vertex(double x, double y, double z, double w) {
    std::cout << "Vertex Constructor called!" << std::endl;
    coordinates = glm::vec4(x,y,z,w);
}

Vertex::Vertex(glm::vec3 pos)
{
    std::cout << "Vertex Constructor called!" << std::endl;
    coordinates = glm::vec4(pos,1);
}


void Vertex::Print() {
    std::cout << "x = " << coordinates.x << ", y = " << coordinates.y << ", z = " << coordinates.z << ", w = " << coordinates.w << std::endl;
}
