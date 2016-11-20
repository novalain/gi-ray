#define _USE_MATH_DEFINES // Needed to run in windows/visual studio
#include "camera.h"
#include "raytracer.h"
#include "ray.h"
#include "scene.h"
// TODO: Remove when we have all point lights in vector
#include <iostream>
#include <sstream>
#include <random>

// TODO: Place these somewhere that makes the most sense and remove some?
const float EPSILON = 0.00001f;
const float gamma_factor = 3.6f;

std::default_random_engine generator3;
std::uniform_real_distribution<float> distribution3(0, 1);

Camera::Camera(Vertex eye_pos1, Vertex eye_pos2, Direction direction, Direction up_vector) :
    direction_(direction), up_vector_(up_vector), framebuffer_(WIDTH, std::vector<Pixel>(HEIGHT)) {
  pos_idx_ = 0;
  eye_pos_[0] = eye_pos1;
  eye_pos_[1] = eye_pos2;

  camera_plane_[0] = Vertex(0.f,-1.f,-1.f);
  camera_plane_[1] = Vertex(0.f, 1.f,-1.f);
  camera_plane_[2] = Vertex(0.f, 1.f, 1.f);
  camera_plane_[3] = Vertex(0.f,-1.f, 1.f);

  delta_ = (camera_plane_[1].y - camera_plane_[0].y)/WIDTH;
  pixel_center_minimum_ = camera_plane_[0].y + delta_/2;
}

void Camera::ChangeEyePos() {
  pos_idx_ = (pos_idx_ == 0) ? 1 : 0 ;
}

double Camera::CalcMaxIntensity() {
  double max_intensity = -1.0;

  //TODO: Parallelize this?
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      max_intensity = fmax(framebuffer_[x][y].get_color().x, max_intensity);
      max_intensity = fmax(framebuffer_[x][y].get_color().y, max_intensity);
      max_intensity = fmax(framebuffer_[x][y].get_color().z, max_intensity);
    }
  }
  return max_intensity;
}

void Camera::NormalizeByMaxIntensity(ImageRgb& image_rgb) {
  double max_intensity = CalcMaxIntensity();
  double normalizing_factor = 255.99/max_intensity;

  // TODO: Parallelize this?
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      image_rgb[x][y][0] = (int) framebuffer_[x][y].get_color().x * normalizing_factor;
      image_rgb[x][y][1] = (int) framebuffer_[x][y].get_color().y * normalizing_factor;
      image_rgb[x][y][2] = (int) framebuffer_[x][y].get_color().z * normalizing_factor;
    }
  }
}

void Camera::NormalizeBySqrt(ImageRgb& image_rgb) {
  float gamma_factor_inv = 1.f / gamma_factor;
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      float r = framebuffer_[x][y].get_color().x;
      float g = framebuffer_[x][y].get_color().y;
      float b = framebuffer_[x][y].get_color().z;
      image_rgb[x][y][0] = (int) 255 * pow(r < 0 ? 0 : r > 1 ? 1 : r, gamma_factor_inv);
      image_rgb[x][y][1] = (int) 255 * pow(g < 0 ? 0 : g > 1 ? 1 : g, gamma_factor_inv);
      image_rgb[x][y][2] = (int) 255 * pow(b < 0 ? 0 : b > 1 ? 1 : b, gamma_factor_inv);
    }
  }
}

void Camera::ClearColorBuffer(ColorDbl clear_color) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      framebuffer_[x][y].set_color(clear_color);
    }
  }
}

void Camera::Render(Scene& scene, int spp /* = 1 */) {
  float factor = ((float)RAND_MAX) / delta_;
  float delta2 = delta_ - (delta_ / 2.f);
  Raytracer raytracer;
  #pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < WIDTH; i++) {
    fprintf(stderr, "\r\tProgress:  %1.2f%%", 100.*i/(WIDTH-1));
    for (int j = 0; j < HEIGHT; j++) {
      ColorDbl temp_color =  COLOR_BLACK;
      for (int s = 0; s < spp; s++ ) {
        float random_y = distribution3(generator3) * delta2;
        float random_z = distribution3(generator3) * delta2;
       
        Vertex pixel_center = Vertex(0, i * delta_ + pixel_center_minimum_ + random_y, j * delta_ + pixel_center_minimum_ + random_z);
        Ray ray = Ray(pixel_center, pixel_center - eye_pos_[pos_idx_]);
        temp_color = temp_color + raytracer.Raytrace(ray, scene, 0);
      }
      framebuffer_[i][j].set_color(temp_color / (float)spp);
    }
  }
}

void Camera::CreateImage(std::string filename, const bool& normalize_intensities) {
  ImageRgb image_rgb (WIDTH,std::vector<std::vector<int>>(HEIGHT,std::vector<int>(3)));
  filename = "results/" + filename + "_" + std::to_string(WIDTH) + "x" + std::to_string(HEIGHT);
  if (normalize_intensities) {
    NormalizeByMaxIntensity(image_rgb);
  } else {
    NormalizeBySqrt(image_rgb);
    std::stringstream ss;
    ss << gamma_factor;
    ss.precision(2);
    filename += "_gamma" + ss.str();
  }
  filename = filename + ".ppm";
  SaveImage(filename.c_str(), image_rgb);
}

void Camera::SaveImage(const char* img_name,
  ImageRgb& image) {
  FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
  (void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
  for (int i = WIDTH - 1; i >= 0; i--) {
    for (int j = HEIGHT - 1; j >= 0; j--) {
      static unsigned char color[3];
      color[0] = image[j][i][0]; // red
      color[1] = image[j][i][1]; // green
      color[2] = image[j][i][2]; // blue
      (void)fwrite(color, 1, 3, fp);
    }
  }
}