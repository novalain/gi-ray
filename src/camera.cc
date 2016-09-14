#include "camera.h"

Camera::Camera() {
  init(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1));
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector) {
  init(position, direction, up_vector);
}

void Camera::init(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector) {
  position_ = position;
  direction_ = direction;
  up_vector_ = up_vector;

  focal_length_ = 1;
  fov_ = 90;
}

double Camera::CalcMaxIntensity() {
  double max_intensity = -1.0;

  //TODO: Parallelize this?
  // Store max_intensity in shared memory? Make sure that it doesn't get
  // overwritten incorrectly because of parallel computations.
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      max_intensity = fmax(framebuffer_[y][x].get_color().x, max_intensity);
      max_intensity = fmax(framebuffer_[y][x].get_color().y, max_intensity);
      max_intensity = fmax(framebuffer_[y][x].get_color().z, max_intensity);
    }
  }
  return max_intensity;
}

template <std::size_t sx, std::size_t sy, std::size_t sz>
void Camera::NormalizeByMaxIntensity(int (&image_rgb)[sx][sy][sz]) {
  double max_intensity = CalcMaxIntensity();
  double normalizing_factor = 255.99/max_intensity;

  // TODO: Parallelize this?
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      // int r = framebuffer_[y][x].get_color().x * normalizing_factor;
      // int g = framebuffer_[y][x].get_color().y * normalizing_factor;
      // int b = framebuffer_[y][x].get_color().z * normalizing_factor;
      image_rgb[y][x][0] = (int) framebuffer_[y][x].get_color().x * normalizing_factor;
      image_rgb[y][x][1] = (int) framebuffer_[y][x].get_color().y * normalizing_factor;
      image_rgb[y][x][2] = (int) framebuffer_[y][x].get_color().z * normalizing_factor;
    }
  }
}

template <std::size_t sx, std::size_t sy, std::size_t sz>
void Camera::NormalizeBySqrt(int (&image_rgb)[sx][sy][sz]) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      int r = (int) sqrt(framebuffer_[y][x].get_color().x);
      int g = (int) sqrt(framebuffer_[y][x].get_color().y);
      int b = (int) sqrt(framebuffer_[y][x].get_color().z);

      image_rgb[y][x][0] = r > 255 ? 255 : r ;
      image_rgb[y][x][1] = g > 255 ? 255 : g ;
      image_rgb[y][x][2] = b > 255 ? 255 : b ;
    }
  }
}

void Camera::ClearColorBuffer(glm::vec3 clear_color) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      framebuffer_[y][x].set_color(clear_color);
    }
  }
}

void Camera::Render() {
  // TODO: implement this
}

void Camera::CreateImage(std::string filename, bool normalize_intensities) {
  int image_rgb[ HEIGHT ][ WIDTH ][ 3 ];
  if(normalize_intensities) {
    NormalizeByMaxIntensity(image_rgb);
  } else {
    NormalizeBySqrt(image_rgb);
  }
  SaveImage("results/imgtest.png", HEIGHT, WIDTH, image_rgb);
}
