#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex
{
private:
    glm::vec4 coordinates_;

    //TODO: Implement the following?
    //glm::vec3 color_;
    //glm::vec2 textureCoord;
public:
    Vertex();

    Vertex(double x, double y, double z, double w);

    Vertex(glm::vec3 pos);

    double GetX() { return coordinates_.x; }
    double GetY() { return coordinates_.y; }
    double GetZ() { return coordinates_.z; }
    double GetW() { return coordinates_.w; }

    glm::vec3 GetPos() { return glm::vec3(coordinates_); }
    glm::vec4 GetCoords() { return coordinates_; }

    void SetX(double x) { coordinates_.x = x; }
    void SetY(double y) { coordinates_.y = y; }
    void SetZ(double z) { coordinates_.z = z; }
    void SetW(double w) { coordinates_.w = w; }

    void Print();

    //TODO: Implment the following ?
    // glm::vec3 GetColor() { return color; }
    // glm::vec2 GetTextureCoord() { return textureCoord; }

protected:
};

#endif // VERTEX_H
