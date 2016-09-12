#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex
{
private:
    glm::vec4 coordinates;

    //TODO: Implement the following?
    //ColorDbl color;
    //Vec2 textureCoord;
public:
    Vertex();

    Vertex(double x, double y, double z, double w);

    Vertex(glm::vec3 pos);

    double GetX() { return coordinates.x; }
    double GetY() { return coordinates.y; }
    double GetZ() { return coordinates.z; }
    double GetW() { return coordinates.w; }

    glm::vec3 GetPos() { return glm::vec3(coordinates); }
    glm::vec4 GetCoords() { return coordinates; }

    void SetX(double x) { coordinates.x = x; }
    void SetY(double y) { coordinates.y = y; }
    void SetZ(double z) { coordinates.z = z; }
    void SetW(double w) { coordinates.w = w; }

    void Print();

    //TODO: Implment the following ?
    // ColorDbl GetColor() { return color; }
    // Vec2 GetTextureCoord() { return textureCoord; }

protected:
};

#endif // VERTEX_H
