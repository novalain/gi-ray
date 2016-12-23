#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <random>

using namespace std;

typedef glm::vec3 vec3;
typedef vec3 Color;
typedef vector<vector<vector<int> > > RgbImage;
typedef vector<vector<vec3> > Framebuffer;

const int WIDTH = 200;
const int HEIGHT = 200;
const int MAX_DEPTH = 2;
const float PI2 =(float) M_PI * 2.f;
const float DIFFUSE_CONTRIBUTION = 0.40f;
const float GAMMA_FACTOR = 3.6f;

// default_random_engine generator18;
// uniform_real_distribution<float> distribution18(0, 1);

float getRandom() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

struct Material {
  Color color;
  float diffuse;
  float specular;
  float opacity;
  float emmitance;

  Material(Color col,
           float dif = 1.f,
           float spe = 0.f,
           float opa = 0.f,
           float emm = 0.f)
      : color(col), diffuse(dif), specular(spe), opacity(opa), emmitance(emm) {}
};

struct Sphere {
  float radius;
  vec3 position;
  Material *material;

  Sphere(vec3 pos, float rad, Material* mat)
      : radius(rad), position(pos), material(mat) {}
};

struct Triangle {
  vec3 *v1;
  vec3 *v2;
  vec3 *v3;
  vec3 normal;
  Material *material;

  Triangle(vec3 *v1_, vec3 *v2_, vec3 *v3_, Material *mat) :
      v1(v1_), v2(v2_), v3(v3_), material(mat) {
    CalculateNormal();
  }

  void CalculateNormal() {
    normal = glm::normalize(glm::cross(*v2 - *v1, *v3 - *v2));
  }
};

struct PointLight {
  vec3 *position;
  float intensity = 1.0f;
  Color color;

  PointLight(vec3 *position, float intensity = 1.0f, Color color = Color(1.f, 1.f, 1.f))
      : position(position), intensity(intensity), color(color) {}
};

struct Ray {
  vec3 origin;
  vec3 direction;

  Ray(vec3 ori, vec3 dir)
      : origin(ori), direction(glm::normalize(dir)) {}
};

void CreateCube(float w,
                float h,
                float d,
                vector<vec3>& va,
                vector<Triangle>& ta,
                vector<Triangle*>& ala,
                Material* mat,
                bool outwards_normals = true,
                vec3 center_position = vec3(0.f, 0.f, 0.f)) {
  int v_idx = va.size();
  // Create Vertices
  {
    float dx = w / 2.f;
    float dy = h / 2.f;
    float dz = d / 2.f;
    float cx = center_position.x;
    float cy = center_position.y;
    float cz = center_position.z;

    va.push_back(vec3(-dx + cx, -dy + cy,  dz + cz)); // left  bottom front
    va.push_back(vec3( dx + cx, -dy + cy,  dz + cz)); // right bottom front
    va.push_back(vec3( dx + cx,  dy + cy,  dz + cz)); // right top    front
    va.push_back(vec3(-dx + cx,  dy + cy,  dz + cz)); // left  top    front
    va.push_back(vec3( dx + cx, -dy + cy, -dz + cz)); // right bottom back
    va.push_back(vec3(-dx + cx, -dy + cy, -dz + cz)); // left  bottom back
    va.push_back(vec3(-dx + cx,  dy + cy, -dz + cz)); // left  top    back
    va.push_back(vec3( dx + cx,  dy + cy, -dz + cz)); // right top    back
  }
  // Create the triangles
  {
    if (outwards_normals) {
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 1], &va[v_idx + 2], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 2], &va[v_idx + 3], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 4], &va[v_idx + 7], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 7], &va[v_idx + 2], mat) ); // right top left
      ta.push_back(Triangle( &va[v_idx + 4], &va[v_idx + 5], &va[v_idx + 6], mat) ); // back bottom right
      ta.push_back(Triangle( &va[v_idx + 4], &va[v_idx + 6], &va[v_idx + 7], mat) ); // back top left
      ta.push_back(Triangle( &va[v_idx + 5], &va[v_idx + 0], &va[v_idx + 3], mat) ); // left bottom right
      ta.push_back(Triangle( &va[v_idx + 5], &va[v_idx + 3], &va[v_idx + 6], mat) ); // left top left
      ta.push_back(Triangle( &va[v_idx + 5], &va[v_idx + 4], &va[v_idx + 1], mat) ); // bottom bottom right
      ta.push_back(Triangle( &va[v_idx + 5], &va[v_idx + 1], &va[v_idx + 0], mat) ); // bottom top left
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 2], &va[v_idx + 7], mat) ); // top bottom right
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 7], &va[v_idx + 6], mat) ); // top top left
    } else {
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 0], &va[v_idx + 2], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 0], &va[v_idx + 3], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 4], &va[v_idx + 1], &va[v_idx + 7], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 7], &va[v_idx + 1], &va[v_idx + 2], mat) ); // right top left
      ta.push_back(Triangle( &va[v_idx + 5], &va[v_idx + 4], &va[v_idx + 6], mat) ); // back bottom right
      ta.push_back(Triangle( &va[v_idx + 6], &va[v_idx + 4], &va[v_idx + 7], mat) ); // back top left
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 5], &va[v_idx + 3], mat) ); // left bottom right
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 5], &va[v_idx + 6], mat) ); // left top left
      ta.push_back(Triangle( &va[v_idx + 4], &va[v_idx + 5], &va[v_idx + 1], mat) ); // bottom bottom right
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 5], &va[v_idx + 0], mat) ); // bottom top left
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 3], &va[v_idx + 7], mat) ); // top bottom right
      ta.push_back(Triangle( &va[v_idx + 7], &va[v_idx + 3], &va[v_idx + 6], mat) ); // top top left
    }
  }
  // If material is an emmiter, add it to the Area Light-source Array (ala)
  if (mat->emmitance > 0.f) {
    int t_idx = ta.size() - 12;
    for (int i = 0; i < 12; i++) {
      ala.push_back(&ta[t_idx + i]);
    }
  }
}

void CreateFourTriangleQuad(float w,
                            float h,
                            vector<vec3>& va,
                            vector<Triangle>& ta,
                            vector<Triangle*>& ala,
                            Material* mat,
                            bool upwards_normal = true,
                            vec3 center_position = vec3(0.f, 0.f, 0.f)) {
  float dx = w / 2.f;
  float dz = h / 2.f;
  float cx = center_position.x;
  float cy = center_position.y;
  float cz = center_position.z;

  int v_idx = va.size();

  va.push_back(vec3(cx - dx, cy, cz + dz)); // left  bottom
  va.push_back(vec3(cx + dx, cy, cz + dz)); // right bottom
  va.push_back(vec3(cx + dx, cy, cz - dz)); // right top
  va.push_back(vec3(cx - dx, cy, cz - dz)); // left  top
  va.push_back(vec3(cx     , cy, cz     )); // center

  // Create the triangles
  {
    if (upwards_normal) {
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 1], &va[v_idx + 4], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 2], &va[v_idx + 4], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 4], &va[v_idx + 4], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 7], &va[v_idx + 4], mat) ); // right top left
    } else {
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 0], &va[v_idx + 4], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 0], &va[v_idx + 4], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 1], &va[v_idx + 4], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 1], &va[v_idx + 4], mat) ); // right top left
    }
  }

  if (mat->emmitance > 0.f) {
    int t_idx = ta.size() - 4;
    for (int i = 0; i < 4; i++) {
      for (int i = 0; i < 4; i++) {
        ala.push_back(&ta[t_idx + i]);
      }
    }
  }
}

void CreateScene(vector<vec3>& va,
                 vector<Triangle>& ta,
                 vector<Material>& ma,
                 vector<PointLight>& pla,
                 vector<Triangle*>& ala) {
  // Default materials (whiteish, blackish, redish, greenish, blueish)
  {
    ma.push_back(Material(vec3(0.9f, 0.9f, 0.9f), 1.0f, 0.0f, 0.0f));
    ma.push_back(Material(vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.0f, 0.0f));
    ma.push_back(Material(vec3(1.0f, 0.5f, 0.5f), 1.0f, 0.0f, 0.0f));
    ma.push_back(Material(vec3(0.5f, 1.0f, 0.5f), 1.0f, 0.0f, 0.0f));
    ma.push_back(Material(vec3(0.5f, 0.5f, 1.0f), 1.0f, 0.0f, 0.0f));
    ma.push_back(Material(vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f, 0.05f));
  }
  // Create the Cornell Box
  {
    CreateCube(10.f, 10.f, 10.f, va, ta, ala, &ma[0], false, vec3(0.f, 0.f, 0.f));
    ta[0].material = &ma[1]; // FRONT
    ta[1].material = &ma[1];
    ta[2].material = &ma[4]; // RIGHT SIDE
    ta[3].material = &ma[4];
    ta[4].material = &ma[0]; // BACK
    ta[5].material = &ma[0];
    ta[6].material = &ma[2]; // LEFT
    ta[7].material = &ma[2];
    // ta[8].material = &ma[0]; // BOTTOM
    // ta[9].material = &ma[0];
    // ta[10].material = &ma[1]; // TOP
    // ta[11].material = &ma[4];
  }

  //Create additional geometry
  {
    CreateCube(2.f, 2.f, 2.f, va, ta, ala, &ma[3], true, vec3(-2.f, -3.0f, -0.0f));
    // CreateCube(2.f, 0.1f, 2.f, va, ta, ala, &ma[5], true, vec3(0.f, 4.75f, 0.f));
    CreateFourTriangleQuad(2.f, 2.f, va, ta, ala, &ma[5], false, vec3(0.f, 4.75f, 0.f));
  }

  //Create Point Lights
  // {
  //   int v_idx = va.size();
  //   va.push_back(vec3(0.f, 4.f, 0.f));
  //   pla.push_back(PointLight( &va[v_idx], 0.6f, Color(1.f, 1.f, 1.f)));
  // }
}

void SaveImage(const char* img_name, Framebuffer &image) {
  float gamma_factor_inv = 1.f / GAMMA_FACTOR;
  FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
  (void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
  for (int i = WIDTH - 1; i >= 0; i--) {
    for (int j = 0 ; j < HEIGHT; j++) {
      float r = image[j][i][0];
      float g = image[j][i][1];
      float b = image[j][i][2];
      static unsigned char color[3];
      // color[0] = (int)(255 * (r < 0 ? 0 : r > 1 ? 1 : r));  // red
      // color[1] = (int)(255 * (g < 0 ? 0 : g > 1 ? 1 : g));  // green
      // color[2] = (int)(255 * (b < 0 ? 0 : b > 1 ? 1 : b));  // blue
      color[0] = (int)(255 * pow(r < 0 ? 0 : r > 1 ? 1 : r, gamma_factor_inv));  // red
      color[1] = (int)(255 * pow(g < 0 ? 0 : g > 1 ? 1 : g, gamma_factor_inv));  // green
      color[2] = (int)(255 * pow(b < 0 ? 0 : b > 1 ? 1 : b, gamma_factor_inv));  // blue
      (void)fwrite(color, 1, 3, fp);
    }
  }
}

void ClearColorBuffer(Color clear_color, Framebuffer &frame_buffer) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      frame_buffer[x][y] = clear_color;
    }
  }
}

// TODO   
// Corresponding MöllerTrumbore For Sphere     

bool MoellerTrumbore(Ray &ray, Triangle &triangle, vec3 &collision_point) {
  vec3 ps = ray.origin; // eye_position
  vec3 D = ray.direction;
  vec3 v0 = *triangle.v1;
  vec3 v1 = *triangle.v2;
  vec3 v2 = *triangle.v3;
  // Simple check if the triangle is facing the camera
  // If non-negative it is facing away from the camera, i.e. not visible from camera
    //if ( glm::dot(normal_,glm::normalize(D)) >= 0) {
    //  return FLT_MAX;
    //}

  // According to the Möller Trumbore intersection algorithm
  vec3 T = ps - v0;
  vec3 E1 = v1 - v0;
  vec3 E2 = v2 - v0;
  vec3 P = glm::cross(D, E2);

  float det = glm::dot(P, E1);

  vec3 Q = glm::cross(T, E1);
  float t = glm::dot(Q,E2) / det;
  float u = glm::dot(P, T) / det;
  float v = glm::dot(Q, D) / det;

  if(u >= 0 && v >= 0 && u+v <= 1 && t > 0.f) {
    collision_point = (1 - u - v)*v0 + u*v1 + v*v2;
    // cout << "\tMöller found a valid collision point!" << endl; //TODO: remove when EVERYTHING is implemented
    return true;
  }
  // cout << "Non-valid collision point!" << endl; //TODO: remove when EVERYTHING is implemented
  return false;
}

float ShadowRay(Ray &ray, vector<Triangle> &ta, float z_buffer) {
  float o_light_factor = 1.0f;
  for (auto& triangle : ta) {
    vec3 temp_collision_point;
    if (MoellerTrumbore(ray, triangle, temp_collision_point)) {
      float z_temp = glm::length(ray.origin - temp_collision_point);
      if (z_temp < z_buffer) {
        o_light_factor *= triangle.material->opacity;
        if (o_light_factor < 0.00001f) {
          return 0.0f;
        }
      }
    }
  }
  return o_light_factor;
}

bool GetClosestIntersectionPoint(Ray& ray,
                                 vector<Triangle>& ta,
                                 float& z_buffer,
                                 Triangle*& triangle_pointer,
                                 vec3& collision_point) {
  bool has_found_intersection_point = false;
  vec3 ray_origin = ray.origin;
  for (auto& triangle : ta) {
    vec3 temp_collision_point;
    if (MoellerTrumbore(ray, triangle, temp_collision_point)) {
      float z_temp = glm::length(ray_origin - temp_collision_point);
      if (z_temp < z_buffer) {
        // cout << "\t\tEven found a closer point!" << endl;
        z_buffer = z_temp;
        has_found_intersection_point = true;
        triangle_pointer = &triangle;
        collision_point = temp_collision_point;
      }
    }
  }
  return has_found_intersection_point;
}

// Forward declaration, just for now.. should probably later be part of a Raytrace class?
Color Raytrace(Ray& ray,
               vector<Triangle>& ta,
               vector<PointLight>& pla,
               vector<Triangle*>& ala,
               unsigned int depth);

Color HandleDirectIllumination(vector<PointLight>& pla,
                               vector<Triangle*>& ala,
                               vector<Triangle>& ta,
                               Triangle* triangle,
                               vec3& collision_point) {
  Color radiance = Color(0.f,0.f, 0.f);
  for(auto &point_light : pla) {
    vec3 relative_vec = *point_light.position - collision_point;
    vec3 light_direction = glm::normalize(relative_vec);

    Ray ray(collision_point + 0.00001f * triangle->normal, light_direction);
    float shadow_value = ShadowRay(ray, ta, glm::length(relative_vec));

    if (shadow_value < 0.0001f) {
      continue;
    }
    float visibilty_factor = glm::dot(triangle->normal, light_direction);
    if( visibilty_factor < 0.f) {
      visibilty_factor = 0.f;
    }
    // TODO: falloff-radius of lights?
    radiance += point_light.intensity * point_light.color *
                    visibilty_factor * shadow_value;
  }

  for(auto &area_light_triangle : ala) {
    // Randomize a point on the triangle using barycentric coordinates
    // float u = distribution(generator);
    // float v = distribution(generator);
    float u = getRandom();
    float v = getRandom();
    float w = u + v;
    if (w > 1.0f) { // todo: this might be biased?
      u /= w;
      v /= w;
    }

    vec3 alp = *(area_light_triangle->v1) * (1 - u - v) +
               *(area_light_triangle->v2) * u +
               *(area_light_triangle->v3) * v;  // area-light point

    vec3 relative_vec = alp - collision_point;
    vec3 light_direction = glm::normalize(relative_vec);

    Ray ray(collision_point + 0.00001f * triangle->normal, light_direction);
    float shadow_value = ShadowRay(ray, ta, glm::length(relative_vec));

    if (shadow_value < 0.0001f) {
      continue;
    }
    float visibilty_factor = glm::dot(triangle->normal, light_direction);
    if( visibilty_factor < 0.f) {
      visibilty_factor = 0.f;
    }
    // TODO: falloff-radius of lights?
    radiance += area_light_triangle->material->emmitance *
                area_light_triangle->material->color * visibilty_factor *
                shadow_value;
  }
  return radiance;
}

// TODO: PointLights aren't doing anything here but is needed in the Raytrace function.. raytracer should probably be a class
Color HandleDiffuse(int depth,
                    vector<Triangle>& ta,
                    Triangle* triangle,
                    vec3& collision_point,
                    vector<PointLight> &pla,
                    vector<Triangle*> &ala) {
  // cout << "\tHandling diffuse case!" << endl;
  vec3 w = triangle->normal;
  // Generalized formula for finding tangent u given a unit length normal vector w
  vec3 u = glm::normalize(glm::cross(fabs(w.x) > 0.1f ? vec3(0.f, 1.f, 0.f) : vec3(1.f, 0.f, 0.f), w));
  vec3 v = glm::cross(w, u);

  // Random direction d within hemisphere
  // Random angle
  // float r1 = PI2 * distribution(generator);
  float r1 = PI2 * getRandom();
  // Random distance from center
  // float r2 = distribution(generator);
  float r2 = getRandom();
  float r2s = sqrtf(r2);
  vec3 d = glm::normalize(u * (float)cos(r1) * r2s + v*(float)sin(r1) * r2s + w * sqrtf(1 - r2));

  vec3 reflection_point_origin = collision_point + w * 0.00001f;
  Ray ray = Ray(reflection_point_origin, d); //, 0.2f * triangle->material.diffuse);
  // cout << "\t\tDiffuse DONE!!" << endl;
  return DIFFUSE_CONTRIBUTION * Raytrace(ray, ta, pla, ala, ++depth);
}

Color Raytrace(Ray& ray,
               vector<Triangle>& ta,
               vector<PointLight>& pla,
               vector<Triangle*>& ala,
               unsigned int depth) {
  // BREAK RECURISVENESS IF MAX DEPTH HAS BEEN REACHED
  Color radiance = Color(0.0f, 0.0f, 0.0f);
  if (depth > MAX_DEPTH) {
    return radiance;
  }

  string temp;
  for (int i = 0; i <= depth; i++) {
    temp += "\t";
  }
  // cout << temp << "Raytrace at depth: " << depth << endl;

  float z_buffer = FLT_MAX;
  Triangle* triangle;
  vec3 collision_point;

  // Get the ray intersection with the nearest surface
  if (GetClosestIntersectionPoint(ray, ta, z_buffer, triangle, collision_point)) {
    // Self emmitance - Required for area light sources
    Color self_emmitance = triangle->material->emmitance * triangle->material->color;

    // Direct Illumination - Required for point lights
    radiance += HandleDirectIllumination(pla, ala, ta, triangle, collision_point);

    if (triangle->material->diffuse > 0.0f) {
      radiance += HandleDiffuse(depth, ta, triangle, collision_point, pla, ala);
    }
    // if (p->get_material().get_specular() > 0.f) {
    //   return HandleSpecular(ray, *p, scene, depth);
    // }
    // if (p->get_material().get_transparence() > 0.f) { // If object has refractive component
    //   return p->get_material().get_color() * HandleRefraction(ray, *p, scene, depth);
    // }
    // if (p->get_material().get_diffuse() > 0.f) {
    //   return HandleDiffuse(ray, *p, scene, depth);
    // }
    //return ray.importance * (intersection_point->get_material().get_emission() + Shade(ray, *intersection_point, scene, depth));
    // return triangle->material->color;
    return self_emmitance + radiance * triangle->material->color;
  }
                                         
  //TODO: SAME THING FOR SPHERES         
                                         
  cerr << "\nLigg här och gnag... " << endl;
  cerr << "EXITING PROGRAM!! FUUUUUUCK THIIIIS SHIIIIT!" << endl;
  exit(0);
  return radiance;
}

void Render(vec3 cam_pos,
            vector<Triangle> &ta,
            vector<PointLight> &pla,
            vector<Triangle*> &ala,
            Framebuffer &frame_buffer,
            float delta,
            float pixel_center_minimum,
            int spp = 1) {
  float delta2 = delta - (delta / 2.f);
  cout << "Rendering!" << endl;
  for (int i = 0; i < WIDTH; i++) {
    // fprintf(stderr, "\r\tProgress:  %1.2f%%", 100. * i / (WIDTH - 1));
    for (int j = 0; j < HEIGHT; j++) {
      Color temp_color = Color(0.f, 0.f, 0.f);
      for (int s = 0; s < spp; s++) {
        // cout << "Rendering pixel (" << i << ", " << j << ")" << endl;
        // float random_x = distribution(generator) * delta2;
        // float random_y = distribution(generator) * delta2;
        float random_x = getRandom() * delta2;
        float random_y = getRandom() * delta2;

        vec3 pixel_random_point =
            vec3(i * delta + pixel_center_minimum + random_x,
                 j * delta + pixel_center_minimum + random_y, 4.0f);
        Ray ray(pixel_random_point, pixel_random_point - cam_pos);
        temp_color = temp_color + Raytrace(ray, ta, pla, ala, 0);
      }
      frame_buffer[i][j] = (temp_color / (float)spp);
    }
    fprintf(stderr, "\r\tRendering progress:  %1.2f%%", 100.*i/(WIDTH-1));
  }
  cout << endl;
}

int main() {
  cout << "GI-Ray to the rescue" << endl;

  // Create camera
  // int width = 200;
  // int height = 200;
  vec3 camera_pos = vec3(0.0f, 0.0f, 4.95f);
  vec3 camera_plane[4];
  camera_plane[0] = vec3(-1.f,-1.f, 4.0f);
  camera_plane[1] = vec3( 1.f,-1.f, 4.0f);
  camera_plane[2] = vec3( 1.f, 1.f, 4.0f);
  camera_plane[3] = vec3(-1.f, 1.f, 4.0f);

  float delta = (camera_plane[1].x - camera_plane[0].x)/WIDTH; // assumes square pixels
  float pixel_center_minimum = camera_plane[0].x + delta/2;

  Framebuffer frame_buffer(WIDTH, vector<vec3>(HEIGHT));

  ClearColorBuffer(Color(1.0f, 0.2f, 0.6f), frame_buffer);

  vector<vec3>        vertex_array;
  vector<Material>    material_array;
  vector<Triangle>    triangle_array;
  vector<PointLight>  point_light_array;
  vector<Triangle*>   area_light_array;

  cout << "Creating the scene..." << endl;
  CreateScene(vertex_array, triangle_array, material_array, point_light_array, area_light_array);

  Render(camera_pos, triangle_array, point_light_array, area_light_array,
         frame_buffer, delta, pixel_center_minimum, 1);

  string filename = "bananskal.ppm";
  cout << "Render DONE!\nWriting to file: " << filename << endl;
  SaveImage(filename.c_str(), frame_buffer);

  cout << "\nGI-Ray finished without any problems" << endl;
  cout << "\nEXITING GI-RAY..." << endl;

  return 0;
}