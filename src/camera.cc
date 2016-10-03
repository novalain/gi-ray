#include "camera.h"
#include "ray.h"
#include "scene.h"
#include "scene_object.h"
// TODO: Remove when we have all point lights in vector
#include "point_light.h"
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
      Vertex pixel_center = Vertex(0, i*delta_ + pixel_center_minimum_, j*delta_ + pixel_center_minimum_);
      Ray ray = Ray(pixel_center, pixel_center - eye_pos_[pos_idx_]);
      ColorDbl out_color = Raytrace(ray, scene);
      framebuffer_[i][j].set_color(out_color);
    }
  }
}

ColorDbl Camera::Shade(Ray& ray, IntersectionPoint& p, Scene& scene) {
  ColorDbl hit_color = COLOR_BLACK;

  if (p.get_material().get_specular() > 0.f) {
    Direction n = glm::normalize(p.get_normal());
    Direction d = ray.get_direction();

    Vertex reflection_point_origin = p.get_position() + n * 0.00001f;
    Direction reflection_direction = d - 2*(glm::dot(d, n))*n;
    Ray reflection_ray = Ray(reflection_point_origin, reflection_direction);
    // return p.get_material().get_specular() * Raytrace(reflection_ray, scene);
    return p.get_material().get_specular() * Raytrace(reflection_ray, scene);
  //} else if (p.get_material().get_transparence() > 0.f) {
  //  return COLOR_PINK;
  }

  // Fully diffuse
  const std::vector<std::unique_ptr<Light>>& lights = scene.get_lights();
  float diffuse_accumulator = 0.f;
  for (auto& light : lights) {
    Direction light_direction = light->get_position() - p.get_position();

    // Set dot product to zero if light is behind the surface
    Direction unit_surface_normal = glm::normalize(p.get_normal());
    Vertex shadow_point_origin = p.get_position() + unit_surface_normal * 0.00001f;

    // Compute shadow ray
    Ray shadow_ray = Ray(shadow_point_origin, light_direction);
    float z_shadow = FLT_MAX;
    IntersectionPoint* p1 = GetClosestIntersectionPointInScene(shadow_ray, scene, z_shadow);
    if (p1 && (z_shadow * z_shadow > glm::dot(light_direction, light_direction))) {
      Direction unit_light_direction = glm::normalize(light_direction);
      float l_dot_n = fmax(0.f, glm::dot(unit_light_direction, unit_surface_normal));
      diffuse_accumulator += light->get_intensity() * p.get_material().get_diffuse() * l_dot_n;
    }
  }
  return diffuse_accumulator * p.get_material().get_color();
}

//TODO: Change this to GetCLosestIntersectionPoint when we start the ray bouncing??
IntersectionPoint* Camera::GetClosestIntersectionPointInScene(Ray& ray, Scene& scene, float& z_buffer) {
  //To make sure we update the z_buffer upon collision.
  const std::vector<std::unique_ptr<SceneObject>>& objects = scene.get_objects();
  for (auto& object : objects) {
    // z_buffer is passed as reference and gets updated
    // TODO: Make void OR make rayintersection return intersection point instead of keeping it in the ray instance
    bool update_pixel_color = object->RayIntersection(ray, z_buffer);
    //bool collision = object->RayIntersection(ray, z_buffer);
    //if (update_pixel_color) {
    //  c = ray.get_color();
    //}
  }
  return ray.get_intersection_point();
}

ColorDbl Camera::Raytrace(Ray& ray, Scene& scene) {
  float z_buffer = FLT_MAX;
  IntersectionPoint* intersection_point = GetClosestIntersectionPointInScene(ray, scene, z_buffer);
  if (intersection_point) {
    return Shade(ray, *intersection_point, scene);
  }
  std::cout << "Ligg här och gnag... " << std::endl;
  return COLOR_BLACK;
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
