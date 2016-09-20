#include "camera.h"
#include "ray.h"
#include "scene.h"
#include "scene_object.h"
#include <iostream>

Camera::Camera() {
}

Camera::Camera(Vertex eye_pos1, Vertex eye_pos2, Direction direction, Direction up_vector) :
    direction_(direction), up_vector_(up_vector), framebuffer_(WIDTH, std::vector<Pixel>(HEIGHT)) {
  pos_idx_ = 0;
  eye_pos_[0] = eye_pos1;
  eye_pos_[1] = eye_pos2;

  camera_plane_[0] = Vertex(0,-1,-1);
  camera_plane_[1] = Vertex(0,1,-1);
  camera_plane_[2] = Vertex(0,1,1);
  camera_plane_[3] = Vertex(0,-1,1);

  delta_ = (camera_plane_[1].y - camera_plane_[0].y)/WIDTH;
  pixel_center_minimum_ = camera_plane_[0].y + delta_/2;
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
      // int r = framebuffer_[x][y].get_color().x * normalizing_factor;
      // int g = framebuffer_[x][y].get_color().y * normalizing_factor;
      // int b = framebuffer_[x][y].get_color().z * normalizing_factor;
      image_rgb[x][y][0] = (int) framebuffer_[x][y].get_color().x * normalizing_factor;
      image_rgb[x][y][1] = (int) framebuffer_[x][y].get_color().y * normalizing_factor;
      image_rgb[x][y][2] = (int) framebuffer_[x][y].get_color().z * normalizing_factor;
    }
  }
}

void Camera::NormalizeBySqrt(ImageRgb& image_rgb) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      int r = (int) sqrt(framebuffer_[x][y].get_color().x);
      int g = (int) sqrt(framebuffer_[x][y].get_color().y);
      int b = (int) sqrt(framebuffer_[x][y].get_color().z);

      image_rgb[x][y][0] = r > 255 ? 255 : r ;
      image_rgb[x][y][1] = g > 255 ? 255 : g ;
      image_rgb[x][y][2] = b > 255 ? 255 : b ;
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

void Camera::Render(Scene& scene) {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      float z_buffer = FLT_MAX; //To make sure we update the z_buffer upon collision.
      Vertex pixel_center = Vertex(0, i*delta_ + pixel_center_minimum_, j*delta_ + pixel_center_minimum_);
      Ray ray = Ray(eye_pos_[pos_idx_],pixel_center);
      const std::vector<std::unique_ptr<SceneObject>>& objects = scene.get_objects();
      for (auto& object : objects) {
        bool update_pixel_color = object->RayIntersection(ray, z_buffer); // z_buffer is passed as reference and gets updated
        if (update_pixel_color) {
          framebuffer_[i][j].set_color(ray.get_color());
        }
      }
    }
  }
}

void Camera::CreateImage(std::string filename, const bool& normalize_intensities) {
  //int image_rgb[ WIDTH ][ HEIGHT ][ 3 ];
  ImageRgb image_rgb (WIDTH,std::vector<std::vector<int>>(HEIGHT,std::vector<int>(3)));
  if (normalize_intensities) {
    NormalizeByMaxIntensity(image_rgb);
  } else {
    NormalizeBySqrt(image_rgb);
  }
  filename = "results/" + filename + ".ppm";
  SaveImage(filename.c_str(), image_rgb);
}

void Camera::SaveImage(const char* img_name,
                        ImageRgb& image) {
    FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
    (void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = WIDTH-1; i >= 0; i-- ) {
      for (int j = HEIGHT-1; j >= 0; j--) {
        static unsigned char color[3];
        color[0] = image[j][i][0]; // red
        color[1] = image[j][i][1]; // green
        color[2] = image[j][i][2]; // blue
        (void)fwrite(color, 1, 3, fp);
      }
    }
  }
