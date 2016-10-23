#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <vector>

#include "commons.h"
#include "pixel.h"
#include "intersection_point.h"

#define WIDTH 250
#define HEIGHT 250

typedef std::vector<std::vector<Pixel>> Framebuffer;

class Scene;

class Camera {
private:
  Vertex eye_pos_[2];
  Vertex camera_plane_[4];

  Direction direction_;
  Direction up_vector_;

  float delta_;
  float pixel_center_minimum_;
  int pos_idx_; // determines which eye_pos_ we are using

  // float focal_length_;
  // float fov_; // field of view
  Framebuffer framebuffer_;

  //TODO: implement a PROPER Z-buffer ;p
  // float zbuffer_[WIDTH][HEIGHT];

  //TODO: aspect ratio?
  //float aspect_ratio_;

  ColorDbl HandleRefraction(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth);
  ColorDbl Raytrace(Ray& ray, Scene& scene, unsigned int depth);
  ColorDbl Shade(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth);
  ColorDbl CalculateDirectIllumination(Ray& ray, IntersectionPoint& p, Scene& scene);
  IntersectionPoint* GetClosestIntersectionPoint(Ray& ray, Scene& scene);
  bool CastShadowRay(Ray& ray, Scene& scene, Direction& light_direction);
 public:
  Camera();
  Camera(Vertex eye_pos1, Vertex eye_pos2, Direction direction, Direction up_vector);

  int get_width() { return WIDTH; }
  int get_height() { return HEIGHT; }
  //TODO: clean this up (or implement it?)
  // float get_fov() { return fov_; }
  // float get_focal_length() { return focal_length_; }
  // Vertex get_position() { return position_; }
  // Direction get_direction() { return direction_; }
  // Direction get_up_vector() { return up_vector_; }

  // void set_focal_length(float f) { focal_length_ = f; }
  // void set_fov(float fov) { fov_ = fov; }
  // void set_position(Vertex pos) { eye = pos; }
  // void set_direction(glm::vec3 dir) { direction_ = dir; }
  // void set_up_vector(glm::vec3 up_vec) { up_vector_ = up_vec; }
  void ChangeEyePos();
  void Render(Scene& scene);
  void ClearColorBuffer(ColorDbl clear_color);
  void CreateImage(std::string filename);
};

#endif // CAMERA_H
