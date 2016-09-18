#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include "commons.h"
#include "pixel.h"
#include <vector>

/**
  Warning: Stack size is OS-dependent and max is 8182 kb on Ubuntu 64
  declaring an array on the stack with width*height*4*3 < 8182000 => width*height < 681833
  i.e size over 825 (width == height) will throw

  TODO: We might want to declare this array on the heap
  (no restrictions on size except when RAM is filled up)
*/
#define WIDTH 1000
#define HEIGHT 1000

typedef std::vector<std::vector<std::vector<int>>> ImageRgb;
typedef std::vector<std::vector<Pixel>> Framebuffer;

class Scene;

class Camera
{
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

  // void init(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector);
  double CalcMaxIntensity();
  void NormalizeByMaxIntensity(ImageRgb& image_rgb);
  void NormalizeBySqrt(ImageRgb& image_rgb);

  static void SaveImage(const char* img_name, ImageRgb& image) {
    FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
    (void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = WIDTH - 1; i >= 0; i--) {
      for (int j = HEIGHT - 1; j >= 0; j--) {
        static unsigned char color[3];
        color[0] = image[j][i][0];  // red
        color[1] = image[j][i][1];  // green
        color[2] = image[j][i][2];  // blue
        (void)fwrite(color, 1, 3, fp);
      }
    }
  }

 public:
  Camera();
  Camera(Vertex eye_pos1, Vertex eye_pos2, Direction direction, Direction up_vector);

  int get_width() { return WIDTH; }
  int get_height() { return HEIGHT; }
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
  void CreateImage(std::string filename, const bool& normalize_intensities);
};

#endif // CAMERA_H
