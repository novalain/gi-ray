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

  double get_x() { return coordinates_.x; }
  double get_y() { return coordinates_.y; }
  double get_z() { return coordinates_.z; }
  double get_w() { return coordinates_.w; }
  glm::vec3 get_pos() { return glm::vec3(coordinates_); }
  glm::vec4 get_coords() { return coordinates_; }

  void set_x(double x) { coordinates_.x = x; }
  void set_y(double y) { coordinates_.y = y; }
  void set_z(double z) { coordinates_.z = z; }
  void set_w(double w) { coordinates_.w = w; }

  void Print();

  //TODO: Implment the following ?
  // glm::vec3 GetColor() { return color; }
  // glm::vec2 GetTextureCoord() { return textureCoord; }

protected:
};

#endif // VERTEX_H
