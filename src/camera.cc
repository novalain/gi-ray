#include "camera.h"
#include "ray.h"
#include "scene.h"
#include "scene_object.h"
// TODO: Remove when we have all point lights in vector
#include "point_light.h"
#include <iostream>

// TODO: Place these somewhere that makes the most sense and remove some?
const float EPSILON = 0.00001f;
const float REFRACTION_FACTOR_OI = REFRACTION_INDEX_AIR / REFRACTION_INDEX_GLASS; // outside->in
const float REFRACTION_FACTOR_IO = REFRACTION_INDEX_GLASS / REFRACTION_INDEX_AIR; // inside->out
const float CRITICAL_ANGLE = asin(REFRACTION_FACTOR_OI);
const unsigned int MAX_DEPTH = 9; // What Max depth makes sense?

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
      ColorDbl out_color = Raytrace(ray, scene, 0);
      framebuffer_[i][j].set_color(out_color);
    }
  }
}

ColorDbl Camera::Shade(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth) {
  // If object has specular component
  if (p.get_material().get_specular() > 0.f) {
    Direction n = glm::normalize(p.get_normal());
    Direction d = ray.get_direction();

    Vertex reflection_point_origin = p.get_position() + n * 0.00001f;
    Direction reflection_direction = d - 2*(glm::dot(d, n))*n;
    Ray reflection_ray = Ray(reflection_point_origin, reflection_direction);
    // return p.get_material().get_specular() * Raytrace(reflection_ray, scene);
    return p.get_material().get_specular() * Raytrace(reflection_ray, scene, depth + 1);
  }
  // If object has refractive component
  if (p.get_material().get_transparence() > 0.f) {
    return HandleRefraction(ray, p, scene, depth);
  }
  // Fully diffuse
  const std::vector<std::unique_ptr<Light>>& lights = scene.get_lights();
  ColorDbl color_accumulator = COLOR_BLACK;
  for (auto& light : lights) {
    Direction light_direction = light->get_position() - p.get_position();

    // Set dot product to zero if light is behind the surface
    Direction unit_surface_normal = glm::normalize(p.get_normal());
    Vertex shadow_point_origin = p.get_position() + unit_surface_normal * 0.00001f;

    // Compute shadow ray
    Ray shadow_ray = Ray(shadow_point_origin, light_direction);
    bool in_shadow = CastShadowRay(shadow_ray, scene, light_direction);

    if (!in_shadow) {
      Direction unit_light_direction = glm::normalize(light_direction);
      float l_dot_n = fmax(0.f, glm::dot(unit_light_direction, unit_surface_normal));
      color_accumulator += light->get_intensity() * light->get_color() * l_dot_n;
    }

  }
  return color_accumulator * p.get_material().get_color() * p.get_material().get_diffuse();
}

// TODO: Refactor later
// Done - according to lecture4 slides
ColorDbl Camera::HandleRefraction(Ray& ray, IntersectionPoint& p, Scene& scene, unsigned int& depth) {
  //TODO: check why these normalizations are NOT redundant
  Direction n = glm::normalize(p.get_normal());
  Direction I = ray.get_direction();
  // assert(std::abs(1.0f - (float)glm::length(p.get_normal())) < EPSILON);
  assert(std::abs(1.0f - (float) glm::length(I)) < EPSILON);
  float I_dot_n = glm::dot(n,I);
  bool is_inside_object = ray.get_refraction_status();

  if ( !is_inside_object ) {
    Direction T = REFRACTION_FACTOR_OI * I - n*(REFRACTION_FACTOR_OI*I_dot_n +
        sqrtf(1 - REFRACTION_FACTOR_OI*REFRACTION_FACTOR_OI * (1 - I_dot_n*I_dot_n)));
    Vertex refraction_point_origin = p.get_position() - n * EPSILON;
    Ray refraction_ray = Ray(refraction_point_origin, T);
    refraction_ray.set_refraction_status(true);
    return p.get_material().get_transparence() * Raytrace(refraction_ray, scene, depth + 1);
  } else { // we are outside of a glass object, trying to go inside
    n = -n; // because we are at the inside of the object now
    // calculate angle between normal and incoming ray direction
    // according to formula: dot(u,v) = |u||v|cos(a) = cos(a), if |u|=|v|=1
    // => a = arccos(dot(u,v)), providing u and v are normalized
    float alpha = acos(I_dot_n);

    if ( alpha > CRITICAL_ANGLE ) { // if total inner reflection
      Vertex inner_reflection_ray_origin = p.get_position() + n * EPSILON;
      Direction reflection_direction = I - 2*(glm::dot(I, n))*n;
      Ray total_inner_reflection_ray = Ray(inner_reflection_ray_origin, reflection_direction);
      total_inner_reflection_ray.set_refraction_status(true);
      return p.get_material().get_transparence() * Raytrace(total_inner_reflection_ray, scene, depth + 1);
    } else if ( CRITICAL_ANGLE == alpha ) {
      std::cerr << "THIS SHOULD NEVER (or at least very rarely) BE PRINTED!!!!!" << std::endl;
      return COLOR_BLACK;
    }
    // else exiting glass object!
    Direction T = REFRACTION_FACTOR_IO * I - n*(-REFRACTION_FACTOR_IO*I_dot_n +
        sqrtf(1 - REFRACTION_FACTOR_IO*REFRACTION_FACTOR_IO * (1 - I_dot_n*I_dot_n)));
    // TODO: this can probably be removed, but might be annoying to debug if wrong
    if ( 1.0f - glm::length(T) > EPSILON ) {
      std::cerr << "THIS SHOULD NOT BE PRINTED! Length of T = " << glm::length(T) << std::endl;
    }
    Vertex outgoing_refraction_ray_origin = p.get_position() - n * EPSILON;
    Ray refraction_ray = Ray(outgoing_refraction_ray_origin, T);
    refraction_ray.set_refraction_status(false);
    return p.get_material().get_transparence() * Raytrace(refraction_ray, scene, depth + 1);
  }
}

//TODO: Change this to GetCLosestIntersectionPoint when we start the ray bouncing??
IntersectionPoint* Camera::GetClosestIntersectionPoint(Ray& ray, Scene& scene) {
  //To make sure we update the z_buffer upon collision.
  const std::vector<std::unique_ptr<SceneObject>>& objects = scene.get_objects();
  IntersectionPoint* return_point;
  float z_buffer = FLT_MAX;
  for (auto& object : objects) {
    IntersectionPoint* p = object->RayIntersection(ray);
    if (p && p->get_z() < z_buffer) {
      z_buffer = p->get_z();
      return_point = p;
    }
  }
  return return_point;
}

bool Camera::CastShadowRay(Ray& ray, Scene& scene, Direction& light_direction) {
  const std::vector<std::unique_ptr<SceneObject>>& objects = scene.get_objects();
  float z_buffer = FLT_MAX;
  for (auto& object : objects) {
    IntersectionPoint* p = object->RayIntersection(ray);
    if (p && p->get_z() < glm::length(light_direction) &&
        p->get_material().get_transparence() == 0.f) {
      return true;
    }
  }
  return false;
}

ColorDbl Camera::Raytrace(Ray& ray, Scene& scene, unsigned int depth) {
  float z_buffer = FLT_MAX;
  IntersectionPoint* intersection_point = GetClosestIntersectionPoint(ray, scene);
  if (intersection_point && depth < MAX_DEPTH) {
    return Shade(ray, *intersection_point, scene, depth);
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
