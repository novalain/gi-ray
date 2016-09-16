#include "camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::Camera(Vertex eye_pos1, Vertex eye_pos2, Direction direction, Direction up_vector) :
    direction_(direction), up_vector_(up_vector) {
  pos_idx_ = 0;
  eye_pos_[0] = eye_pos1;
  eye_pos_[1] = eye_pos2;

  camera_plane_[0] = Vertex(0,-1,-1);
  camera_plane_[1] = Vertex(0,1,-1);
  camera_plane_[2] = Vertex(0,1,1);
  camera_plane_[3] = Vertex(0,-1,1);

  delta_ = (camera_plane_[1].y - camera_plane_[0].y)/WIDTH;
  pixel_center_minimum_ = camera_plane_[0].y + delta_/2;
  std::cout << "delta = " << delta_ << "min = " << pixel_center_minimum_ << std::endl;
}

void Camera::ChangeEyePos() {
  pos_idx_ = (pos_idx_ == 0) ? 1 : 0 ;
  // If we want more than 2 positions to switch between we could use a switch
  // switch (pos_idx_)
  // {
  // case 0:
  //   pos_idx = 1;
  //   break;
  // case 1:
  //   pos_idx = 0;
  //   break;
  // default:
  //   break;
  // }
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

void Camera::ClearColorBuffer(ColorDbl clear_color) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      framebuffer_[y][x].set_color(clear_color);
    }
  }
}

void Camera::Render() {
  // TODO: implement this
  for(int i = 0; i < WIDTH; i++) {
    for(int j = 0; j < HEIGHT; j++) {
      bool collision = false;
      Vertex pixel_center = Vertex(0, i*delta_ + pixel_center_minimum_, j*delta_ + pixel_center_minimum_);
      Ray ray = Ray(&eye_pos_[pos_idx_],&pixel_center);
      for(int tri = 0; tri < scene_->get_num_of_triangles(); tri++) {
        //TODO: uncomment this
        collision = (*scene_).get_triangles()[tri].RayIntersection(&ray);
        if(collision) {
          framebuffer_[j][i].set_color(ray.get_color());
          break;
        }
      }
    }
  }
}

void Camera::CreateImage(std::string filename, bool normalize_intensities) {
  int image_rgb[ HEIGHT ][ WIDTH ][ 3 ];
  if(normalize_intensities) {
    NormalizeByMaxIntensity(image_rgb);
  } else {
    NormalizeBySqrt(image_rgb);
  }
  filename = "results/" + filename + ".png";
  SaveImage(filename.c_str(), HEIGHT, WIDTH, image_rgb);
}
