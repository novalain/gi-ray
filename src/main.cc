// TODO: separate depth variables for diffuse, specular and transparent
// TODO: change all HandleSpecular, HandleDiffuse etc. to voids or bools.. pass color by reference?
// TODO: move depth into ray class?

#define _USE_MATH_DEFINES // Needed to run in windows/visual studio

// (size x size x samples x depth)

// OS X
// 256 x 256 x 200 x 2 => 127s (without -O3)
// 256 x 256 x 200 x 2 => 16s
// 512 x 512 x 1000 x 2 => 346s
// 512 x 512 x 5000 x 2 => 1696s

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <random>
#include <omp.h>
#include <ctime>
#ifdef _OPENMP
  #include <omp.h>
#endif
#include <cstring>

using namespace std;

typedef glm::vec3 vec3;
typedef vec3 Color;
typedef vector<vector<vec3> > Framebuffer;

const int WIDTH = 200;
const int HEIGHT = 200;
const int SAMPLES = 5000;
const int MAX_DEPTH = 4; // 0 = only point directly seen by camera
const float EPSILON = 0.00001f;
const float PI2 =(float) M_PI * 2.0f;
const float DIFFUSE_CONTRIBUTION = .80f;
float GAMMA_FACTOR = 3.5f;

// Refractive indecis: air = 1.0, glass = 1.5
const float REFRACTION_INDEX_AIR = 1.0f;
const float REFRACTION_INDEX_GLASS = 1.5f;
const float REFRACTIVE_FACTOR_ENTER_GLASS = REFRACTION_INDEX_AIR / REFRACTION_INDEX_GLASS;
const float REFRACTIVE_FACTOR_EXIT_GLASS = REFRACTION_INDEX_GLASS / REFRACTION_INDEX_AIR;
const float CRITICAL_ANGLE = asin(REFRACTIVE_FACTOR_EXIT_GLASS);

default_random_engine generator;
uniform_real_distribution<float> distribution(0, 1);

struct Material {
  Color color;
  float diffuse;
  float specular;
  float transparency;
  float emmitance;

  Material(Color col,
           float dif = 1.0f,
           float spe = 0.0f,
           float opa = 0.0f,
           float emm = 0.0f)
      : color{col}, diffuse{dif}, specular{spe}, transparency{opa}, emmitance{emm} {
    // Prevent material from ADDING light (unless it is an emmiter)
    float sum = diffuse + specular + transparency;
    if (sum > 1.0f) {
      diffuse /= sum;
      specular /= sum;
      transparency /= sum;
    }
  }
};

struct Sphere {
  float radius;
  vec3* position;
  Material* material;

  Sphere(vec3* pos, float& rad, Material* mat)
      : radius(rad), position(pos), material(mat) {}
};

struct Triangle {
  vec3* v1;
  vec3* v2;
  vec3* v3;
  vec3 normal;
  Material* material;

  Triangle(vec3* v1_, vec3* v2_, vec3* v3_, Material* mat) :
      v1{v1_}, v2{v2_}, v3{v3_}, material{mat} {
    CalculateNormal();
  }

  void CalculateNormal() {
    normal = glm::normalize(glm::cross((*v2) - (*v1), (*v3) - (*v2)));
  }
};

struct PointLight {
  vec3* position;
  float intensity = 1.0f;
  Color color;

  PointLight(vec3 *position, float intensity = 1.0f, Color color = Color(1.0f, 1.0f, 1.0f))
      : position(position), intensity(intensity), color(color) {}
};

struct Ray {
  vec3 origin;
  vec3 direction;

  Ray(vec3 ori, vec3 dir)
      : origin(ori), direction(glm::normalize(dir)) {}
};

/** Slightly modified version from opengl-tutorial.org,
    loads a simple blender model */
bool loadOBJ(const char* path,
             std::vector<glm::vec3>& va,
             std::vector<Triangle>& ta,
             std::vector<Material>& ma) {
  printf("Loading OBJ file %s...\n", path);

  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  FILE* file = fopen(path, "r");
  if (file == NULL) {
    printf(
        "Impossible to open the file ! Are you in the right path ? See "
        "Tutorial 1 for details\n");
    getchar();
    return false;
  }

  while (1) {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break;  // EOF = End Of File. Quit the loop.

    if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uv.y = -uv.y;  // Invert V coordinate since we will only use DDS texture,
                     // which are inverted. Remove if you want to use TGA or BMP
                     // loaders.
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9) {
        printf(
            "File can't be read by our simple parser :-( Try exporting with "
            "other options\n");
        return false;
      }

      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    } else {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      fgets(stupidBuffer, 1000, file);
    }
  }

  std::vector<glm::vec3> vertices;
  int count = 0;
  // For each vertex of each triangle
  for (unsigned int i = 0; i < vertexIndices.size(); i++) {
    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[i];
    unsigned int uvIndex = uvIndices[i];
    unsigned int normalIndex = normalIndices[i];

    // Get the attributes thanks to the index
    glm::vec3 vertex = temp_vertices[vertexIndex - 1];
    glm::vec2 uv = temp_uvs[uvIndex - 1];
    glm::vec3 normal = temp_normals[normalIndex - 1];
    // Put the attributes in buffers
    vertices.push_back(vertex);

    if (count == 2) {
      int v_idx = va.size();
      va.push_back(vertices[i - 0] * 2.f);
      va.push_back(vertices[i - 1] * 2.f);
      va.push_back(vertices[i - 2] * 2.f);
      ta.push_back(Triangle(&va[v_idx + 0], &va[v_idx + 1], &va[v_idx + 2],
                            &ma[6]));  // front bottom right
      count = 0;
      continue;
    }
    count++;
  }
  return true;
}

void CreateSphere(float r,
                  vector<vec3>& va,
                  vector<Sphere>& sa,
                  Material* mat,
                  vec3 center_position = vec3(0.0f, 0.0f, 0.0f)) {
  int v_idx = va.size();
  va.push_back(vec3(center_position.x, center_position.y, center_position.z));
  sa.push_back(Sphere(&va[v_idx], r, mat));
}

void CreateCube(float w,
                float h,
                float d,
                vector<vec3>& va,
                vector<Triangle>& ta,
                vector<Triangle*>& ala,
                Material* mat,
                bool outwards_normals = true,
                vec3 center_position = vec3(0.0f, 0.0f, 0.0f)) {
  int v_idx = va.size();
  // Create Vertices

    float dx = w / 2.0f;
    float dy = h / 2.0f;
    float dz = d / 2.0f;
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

    vec3 v0 = vec3(-dx + cx, -dy + cy,  dz + cz);
    vec3 v1 = vec3( dx + cx, -dy + cy,  dz + cz);
    vec3 v2 = vec3( dx + cx,  dy + cy,  dz + cz);
    vec3 v3 = vec3(-dx + cx,  dy + cy,  dz + cz);
    vec3 v4 = vec3( dx + cx, -dy + cy, -dz + cz);
    vec3 v5 = vec3(-dx + cx, -dy + cy, -dz + cz);
    vec3 v6 = vec3(-dx + cx,  dy + cy, -dz + cz);
    vec3 v7 = vec3( dx + cx,  dy + cy, -dz + cz);
  
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
  {
    if (mat->emmitance > 0.0f) {
      int t_idx = ta.size() - 12;
      for (int i = 0; i < 12; i++) {
        ala.push_back(&ta[t_idx + i]);
      }
    }
  }
}

void CreateFourTriangleQuad(float w,
                            float h,
                            vector<vec3> &va,
                            vector<Triangle> &ta,
                            vector<Triangle*> &ala,
                            Material* mat,
                            bool upwards_normal = true,
                            vec3 center_position = vec3(0.0f, 0.0f, 0.0f)) {
  float dx = w / 2.0f;
  float dz = h / 2.0f;
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
    if (upwards_normal) { //TODO byt plats på y & z?
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 1], &va[v_idx + 4], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 2], &va[v_idx + 4], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 3], &va[v_idx + 4], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 0], &va[v_idx + 4], mat) ); // right top left
    } else {
      ta.push_back(Triangle( &va[v_idx + 1], &va[v_idx + 0], &va[v_idx + 4], mat) ); // front bottom right
      ta.push_back(Triangle( &va[v_idx + 2], &va[v_idx + 1], &va[v_idx + 4], mat) ); // front top left
      ta.push_back(Triangle( &va[v_idx + 3], &va[v_idx + 2], &va[v_idx + 4], mat) ); // right bottom right
      ta.push_back(Triangle( &va[v_idx + 0], &va[v_idx + 3], &va[v_idx + 4], mat) ); // right top left
    }
  }

  if (mat->emmitance > 0.0f) {
    int t_idx = ta.size() - 4;
    for (int i = 0; i < 4; i++) {
        ala.push_back(&ta[t_idx + i]);
    }
  }
}

void CreateScene(vector<vec3> &va,
                 vector<Triangle> &ta,
                 vector<Sphere> &sa,
                 vector<Material> &ma,
                 vector<PointLight> &pla,
                 vector<Triangle*> &ala) {
  // Default materials (whiteish, blackish, redish, greenish, blueish. etc..)
  {
    ma.push_back(Material(vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f));       // 0 WHITE
    ma.push_back(Material(vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.0f, 0.0f));       // 1 BLACKISH
    ma.push_back(Material(vec3(1.0f, 0.2f, 0.2f), 1.0f, 0.0f, 0.0f));       // 2 REDISH
    ma.push_back(Material(vec3(0.2f, 1.0f, 0.2f), 1.0f, 0.0f, 0.0f));       // 3 GREENISH
    ma.push_back(Material(vec3(0.2f, 0.2f, 1.0f), 1.0f, 0.0f, 0.0f));       // 4 BLUEISH
    ma.push_back(Material(vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f, 0.05f)); // 5 WHITE EMMITER
    ma.push_back(Material(vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, 0.0f));       // 6 PERFECT MIRROR
    ma.push_back(Material(vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f, 0.0f));     // 7 RUBY
    ma.push_back(Material(vec3(0.0f, 1.0f, 0.0f), 0.4f, 0.6f, 0.0f));       // 8 EMERALD
    ma.push_back(Material(vec3(0.0f, 0.0f, 1.0f), 0.7f, 0.3f, 0.0f));       // 9 SAPPHIRE
    ma.push_back(Material(vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 1.0f));       //10 GLASS
  }
  //Create additional geometry
  {
    //CreateCube(2.25f, 2.25f, 2.25f, va, ta, ala, &ma[9], true, vec3(-2.0f, -1.8f, -1.0f));
    // CreateCube(1.4f, 1.4f, 1.4f, va, ta, ala, &ma[2], true, vec3(2.0f, -1.6f, -1.0f));
    // CreateCube(2.0f, 0.1f, 2.0f, va, ta, ala, &ma[5], true, vec3(0.0f, 4.05f, 0.0f));
    CreateFourTriangleQuad(2.0f, 2.0f, va, ta, ala, &ma[5], false, vec3(0.0f, 4.95f, 0.0f));
    CreateSphere(1.7f, va, sa, &ma[10], vec3(-2.6f, -2.1f, -0.7f));
    //CreateSphere(1.7f, va, sa, &ma[7], vec3(3.0f, -3.25f, -1.6f));
    //CreateSphere(1.3f, va, sa, &ma[6], vec3(0.f, 1.f, -1.5f));
    // Create Monkey
    //bool res =
      //loadOBJ("data/suzanne.obj", va, ta, ma);
  }

  // Create the Cornell Box
  {
    CreateCube(10.0f, 10.0f, 10.0f, va, ta, ala, &ma[0], false, vec3(0.0f, 0.0f, 0.0f));
    int t_idx = ta.size()-12;
    // ta[t_idx+0].material = &ma[6]; // FRONT
    // ta[t_idx+1].material = &ma[6];
    ta[t_idx+2].material = &ma[4]; // RIGHT SIDE
    ta[t_idx+3].material = &ma[4];
    ta[t_idx+4].material = &ma[3]; // BACK
    ta[t_idx+5].material = &ma[3];
    ta[t_idx+6].material = &ma[2]; // LEFT
    ta[t_idx+7].material = &ma[2];
    // ta[t_idx+8].material = &ma[0]; // BOTTOM
    // ta[t_idx+9].material = &ma[0];
    // ta[t_idx+10].material = &ma[6]; // TOP
    // ta[t_idx+11].material = &ma[6];
  }

  //Create Point Lights
  {
    // int v_idx = va.size();
    // va.push_back(vec3(0.0f, 4.0f, 0.0f));
    // pla.push_back(PointLight( &va[v_idx], 0.8f, Color(1.0f, 1.0f, 1.0f)));
  }
}

void SaveImage(const char* img_name, Framebuffer &image) {
  float gamma_factor_inv = 1.0f / GAMMA_FACTOR;
  FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
  (void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
  for (int i = WIDTH - 1; i >= 0; i--) {
    for (int j = 0 ; j < HEIGHT; j++) {
      static unsigned char color[3];
      for (int c = 0; c < 3; c++) {
        float r = image[j][i][c];
        color[c] = (int)(255 * pow(r < 0.0f ? 0.0f : r > 1.0f ? 1.0f : r, gamma_factor_inv));
        if (color[c] < 0 || color[c] > 255)
          cout << "\nWTF!!\n" << endl;
      }
      /*float r = image[j][i][0];
      float g = image[j][i][1];
      float b = image[j][i][2];
      static unsigned char color[3];
      color[0] = (int)(255 * pow(r < 0.0f ? 0.0f : r > 1.0f ? 1.0f : r, gamma_factor_inv));  // red
      color[1] = (int)(255 * pow(g < 0.0f ? 0.0f : g > 1.0f ? 1.0f : g, gamma_factor_inv));  // green
      color[2] = (int)(255 * pow(b < 0.0f ? 0.0f : b > 1.0f ? 1.0f : b, gamma_factor_inv));  // blue
      */

      (void)fwrite(color, 1, 3, fp);
    }
  }
  fclose(fp); // no more black lines
}

void ClearColorBuffer(Color clear_color, Framebuffer &frame_buffer) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      frame_buffer[x][y] = clear_color;
    }
  }
}   

bool MoellerTrumbore(const Ray &ray, const Triangle &triangle, vec3 &collision_point) {
  vec3 ps = ray.origin;
  vec3 D = ray.direction;
  vec3 v0 = *(triangle.v1);
  vec3 v1 = *(triangle.v2);
  vec3 v2 = *(triangle.v3);

  // According to the Möller Trumbore intersection algorithm
  vec3 T = ps - v0;
  vec3 E1 = v1 - v0;
  vec3 E2 = v2 - v0;
  vec3 P = glm::cross(D, E2);

  float det = glm::dot(P, E1);
  if (det < EPSILON && det > -EPSILON) {
    return false;
  }
  float inv_det = 1.f / det;

  vec3 Q = glm::cross(T, E1);
  float t = glm::dot(Q,E2) * inv_det;
  float u = glm::dot(P, T) * inv_det;
  float v = glm::dot(Q, D) * inv_det;

  if (!(u < 0) && !(v < 0) && u+v < 1.000001f && t > EPSILON) {
  //if (u >= 0.0f && v >= 0.0f && u + v <= 1.000001f && t > 0.0f) {

    collision_point = (1.0f - u - v) * v0 + u * v1 + v * v2;
    return true;
  }
  return false;
}

float ShadowRay(const Ray& ray,
                const vector<Triangle>& ta,
                const float& z_buffer,
                Triangle* area_light_triangle = nullptr) {
  float o_light_factor = 1.0f;
  int t_length = ta.size();
  for (auto& triangle : ta) {
    vec3 temp_collision_point;
    if (&triangle != &(*area_light_triangle) &&
        MoellerTrumbore(ray, triangle, temp_collision_point)) {
      float z_temp = glm::length(ray.origin - temp_collision_point);
      if (z_temp < z_buffer) {
        o_light_factor *= triangle.material->transparency;
        if (o_light_factor < 0.00001f) {
          return 0.0f;
        }
      }
    }
  }
  return o_light_factor;
}

bool SolveQuadratic(const float& b,
                    const float& c,
                    float& x0,
                    float& x1) {
  float discr = b * b - 4 * c;
  if (discr < 0) {
    return false;
  } else if (discr == 0) {
    x0 = x1 = -0.5f * b;
  } else {
    float q = (b > 0) ? -0.5f * (b + sqrt(discr)) : -0.5f * (b - sqrt(discr));
    x0 = q;
    x1 = c / q;
  }
  if (x0 > x1) {
    std::swap(x0, x1);
  }
  return true;
}

bool SphereIntersection(const Ray &ray, const Sphere &sphere, vec3 &collision_point) {
  vec3 L = ray.origin - *sphere.position;
  vec3 dir = ray.direction;
  float radius2 = sphere.radius * sphere.radius;
  //float a = glm::dot(dir, dir);
  float b = 2 * glm::dot(dir, L);
  float c = glm::dot(L, L) - radius2;
  float t0, t1;
  if (!SolveQuadratic(b, c, t0, t1)) {
    return false;
  }
  if (t0 < 0) {
    if (t1 < 0) {
      return false;
    }
    t0 = t1;
  }
  collision_point = ray.origin + dir * t0;
  return true;
  // return std::make_unique<IntersectionPoint>(intersection_point, normal, material_, t0);
}

bool SphereIntersectionNEW(const Ray &ray, const Sphere &sphere, vec3 &collision_point) {
  vec3 l = ray.direction;
  float r2 = sphere.radius * sphere.radius;
  vec3 oc = ray.origin - *sphere.position;
  float loc = glm::dot(l, oc);
  float sr = pow(loc, 2.f) - glm::dot(oc, oc) + r2;
  // cant take square root of a negative number => no solution
  if (sr < 0) {//< EPSILON) {
    return false;
  } else if (sr < EPSILON) {
    collision_point = ray.origin + l * -loc;
    return true;
  }
  sr = sqrt(sr);
  float d1 = -loc + sr;
  float d2 = -loc - sr;
  
  if (d2 > EPSILON) {
    collision_point = ray.origin + l * d2;
  }  else {
    if (d1 < EPSILON) {
      return false;
    }
    collision_point = ray.origin + l * d1;
  }

  return true;
}

bool GetClosestIntersectionPoint(const Ray &ray,
                                 const vector<Triangle>& ta,
                                 const vector<Sphere>& sa,
                                 float &z_buffer,
                                 vec3& normal,
                                 Material* & mat,
                                 vec3 &collision_point) {
  bool has_found_intersection_point = false;
  vec3 ray_origin = ray.origin;
  int t_length = ta.size();
  for (auto& sphere : sa) {
    vec3 temp_collision_point;
    if (SphereIntersection(ray, sphere, temp_collision_point)) {
      float z_temp = glm::length(ray_origin - temp_collision_point);
      if (z_temp < z_buffer) {
        z_buffer = z_temp;
        has_found_intersection_point = true;
        collision_point = temp_collision_point;
        normal = glm::normalize(collision_point - *sphere.position);
        mat = sphere.material;
      }
    }
  }
  for (auto& triangle : ta) {
    vec3 temp_collision_point;
    if (MoellerTrumbore(ray, triangle, temp_collision_point)) {
      float z_temp = glm::length(ray_origin - temp_collision_point);
      if (z_temp < z_buffer) {
        z_buffer = z_temp;
        has_found_intersection_point = true;
        collision_point = temp_collision_point;
        normal = triangle.normal;
        mat = triangle.material;
      }
    }
  }
  return has_found_intersection_point;
}

// Forward declaration, just for now.. should probably later be part of a Raytrace class?
Color Raytrace(const Ray& ray,
               const vector<Triangle> &ta,
               const vector<Sphere> &sa,
               const vector<PointLight> &pla,
               const vector<Triangle*> &ala,
               unsigned int depth);

Color HandleDirectIllumination(const vector<PointLight> &pla,
                               const vector<Triangle*> &ala,
                               const vector<Triangle> &ta,
                               Triangle* triangle,
                               const vec3 &collision_point) {
  Color radiance = Color(0.0f, 0.0f, 0.0f);
  // TODO: create separate function that take in the pos, intensity & color?
  for(auto &point_light : pla) {
    vec3 relative_vec = *point_light.position - collision_point;
    vec3 light_direction = glm::normalize(relative_vec);

    float visibilty_factor = glm::dot(triangle->normal, light_direction);
    if( visibilty_factor < 0.0f) {
      continue;
    }

    Ray ray(collision_point + 0.00001f * triangle->normal, light_direction);
    float shadow_value = ShadowRay(ray, ta, glm::length(relative_vec));

    if (shadow_value < 0.00001f) {
      continue;
    }
    // TODO: falloff-radius of lights?
    radiance += point_light.intensity * point_light.color *
                    visibilty_factor * shadow_value;
  }

  for(auto &area_light_triangle : ala) {
    // Randomize a point on the triangle using barycentric coordinates
    float u = distribution(generator);
    float v = distribution(generator);
    float w = distribution(generator);
    float sum = u + v + w;
    if (sum > 1.0f) { // todo: this might be biased?
      u /= sum;
      v /= sum;
      w /= sum;
    }

    vec3 alp = *(area_light_triangle->v1) * w +
               *(area_light_triangle->v2) * u +
               *(area_light_triangle->v3) * v;  // area-light point

    vec3 relative_vec = alp - collision_point;
    vec3 light_direction = glm::normalize(relative_vec);

    float visibilty_factor = glm::dot(triangle->normal, light_direction);
    if( visibilty_factor < 0.0f) {
      continue; //visibilty_factor = 0.0f;
    }

    Ray ray(collision_point + 0.00001f * triangle->normal, light_direction);
    float shadow_value =
        ShadowRay(ray, ta, glm::length(relative_vec), area_light_triangle);

    if (shadow_value < 0.00001f) {
      continue;
    }
    // TODO: falloff-radius of lights?
    radiance += area_light_triangle->material->emmitance *
                area_light_triangle->material->color * visibilty_factor /*
                shadow_value*/;
  }
  return radiance;
}

// TODO: PointLights aren't doing anything here but is needed in the Raytrace function.. raytracer should probably be a class
Color HandleDiffuse(int depth,
                    const vector<Triangle> &ta,
                    const vector<Sphere> &sa,
                    const vec3& normal,
                    const vec3& collision_point,
                    const vector<PointLight> &pla,
                    const vector<Triangle*> &ala) {
  vec3 w = normal;
  // Generalized formula for finding tangent u given a unit length normal vector w
  vec3 u = glm::normalize(glm::cross(fabs(w.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f), w));
  vec3 v = glm::cross(w, u);

  // Random direction d within hemisphere
  // Random angle
  float r1 = PI2 * distribution(generator);
  // Random distance from center
  float r2 = distribution(generator);
  float r2s = sqrtf(r2);
  vec3 d = glm::normalize(u * (float)cos(r1) * r2s + v*(float)sin(r1) * r2s + w * sqrtf(1.0f - r2));

  vec3 reflection_point_origin = collision_point + w * 0.00001f;
  Ray ray = Ray(reflection_point_origin, d);
  return DIFFUSE_CONTRIBUTION * Raytrace(ray, ta, sa, pla, ala, ++depth);
}

Color HandleSpecular(const Ray& ray,
  int depth,
  const vector<Triangle>& ta,
  const vector<Sphere>& sa,
  const vec3& normal,
  const vec3& collision_point,
  const vector<PointLight>& pla,
  const vector<Triangle*>& ala) {
  vec3 n = normal;
  vec3 d = ray.direction;
  // TODO: THIS IS AN ISSUE! it should never happen, right?
  if (glm::dot(n, d) > 0) {
    n = -n;
    //cout << "REVERSING NORMAL IN SPECULAR - THIS MUST BE WRONG - SHOULD NEVER HAPPEN?!" << endl;
  } else {
    //cout << "NOT REVERSING NORMAL! THIS SHOULD BE CORRECT?!" << endl;
  }

  vec3 reflection_point_origin = collision_point + n * 0.00001f;
  vec3 reflection_direction = d - 2.f * (glm::dot(d, n))*n;
  Ray reflection_ray = Ray(reflection_point_origin, reflection_direction);
  return Raytrace(reflection_ray, ta, sa, pla, ala, ++depth);
}

Color HandleRefraction(const Ray& ray,
                       int depth,
                       const vector<Triangle>& ta,
                       const vector<Sphere>& sa,
                       const vec3& normal,
                       const vec3& collision_point,
                       const vector<PointLight>& pla,
                       const vector<Triangle*>& ala) {
  vec3 n = normal; // outwards normal
  vec3 I = ray.direction;
  float I_dot_n = glm::dot(n,I);
  float refractive_factor, n1, n2, alpha;
  
  // From outside going in
  if (I_dot_n < 0) {
    n1 = REFRACTION_INDEX_AIR;
    n2 = REFRACTION_INDEX_GLASS;
    refractive_factor = REFRACTIVE_FACTOR_ENTER_GLASS;
    alpha = acos(-I_dot_n);
  } else { // From inside going out
    n1 = REFRACTION_INDEX_GLASS;
    n2 = REFRACTION_INDEX_AIR;
    refractive_factor = REFRACTIVE_FACTOR_EXIT_GLASS;
    n = -n; // inwards normal
    alpha = acos(I_dot_n);
    I_dot_n *= -1;// glm::dot(n, -I);
    if ( alpha > CRITICAL_ANGLE ) { // total inner reflection occurs (only reflection term)
      const vec3 reflection_normal = n;
      return HandleSpecular(ray, depth, ta, sa, reflection_normal, collision_point, pla, ala);
    }
  }
  vec3 Tvec = refractive_factor * I + n * (-refractive_factor * I_dot_n -
      sqrtf(1.f - refractive_factor * refractive_factor * (1.f - I_dot_n * I_dot_n)));
  vec3 refraction_point_origin = collision_point - n * EPSILON; //TODO: Changed to addition from subtraction.. wrong or correct?
  Ray refraction_ray = Ray(refraction_point_origin, Tvec);

  // Distribute radiance in reflection and refraction directions
  float c = cos(alpha);
  float n1c = n1*c;
  float n2c = n2*c;
  float sr = sqrt(1.0f - pow(refractive_factor * sin(alpha), 2.f));
  float n2sr = n2*sr;
  float n1sr = n1*sr;
  float Rs = pow((n1c - n2sr) / (n1c + n2sr), 2.f);
  float Rp = pow((n1sr - n2c) / (n1sr + n2c), 2.f);
  float R = (Rs + Rp) / 2.f;
  float T = 1.f - R;
  const vec3 reflection_normal = n; // FULLÖSNING
  if (T < EPSILON) {
    //cout << "\nT: " << T << "\tR: " << R << endl;
    return HandleSpecular(ray, depth, ta, sa,  reflection_normal, collision_point, pla, ala);
  } else if (R < EPSILON) {
    cout << " \nR: " << R << "\tT: " << T << endl;
    return Raytrace(refraction_ray, ta, sa, pla, ala, ++depth);
  }

  return T * Raytrace(refraction_ray, ta, sa, pla, ala, ++depth) + R * HandleSpecular(ray, depth, ta, sa, reflection_normal, collision_point, pla, ala);
}

Color Raytrace(const Ray& ray,
               const vector<Triangle>& ta,
               const vector<Sphere>& sa,
               const vector<PointLight>& pla,
               const vector<Triangle*>& ala,
               unsigned int depth) {
  Color radiance = Color(0.0f, 0.0f, 0.0f);

  // BREAK RECURISVENESS IF MAX DEPTH HAS BEEN REACHED
  if (depth > MAX_DEPTH) {
    return Color(0.0f,1.0f,0.0f);// radiance;
  }

  float z_buffer = FLT_MAX;
  vec3 normal;
  Material* mat;
  vec3 collision_point;

  // Get the ray intersection with the nearest surface
  if (GetClosestIntersectionPoint(ray, ta, sa, z_buffer, normal, mat, collision_point)) {
    float diffuse = mat->diffuse;
    float specular = mat->specular;
    float transparency = mat->transparency;
    float emmitance = mat->emmitance;
    const vec3 cp = collision_point;

    Color self_emmitance = Color(0.0f, 0.0f, 0.0f);
    if(emmitance > EPSILON) {
      self_emmitance = (1.0f + emmitance) * mat->color;
    }

    // Direct Illumination - Required for point lights
    //radiance += HandleDirectIllumination(pla, ala, ta, triangle, collision_point);

    if (diffuse > EPSILON) {//0.0f) {
      radiance += diffuse * HandleDiffuse(depth, ta, sa, normal, cp, pla, ala) *  mat->color;
    }

    if (specular > EPSILON) {//0.0f) {
      radiance += specular * HandleSpecular(ray, depth, ta, sa, normal, cp, pla, ala);
    }
    if (transparency > EPSILON) {//0.0f) {
      radiance += transparency * HandleRefraction(ray, depth, ta, sa, normal, cp, pla, ala);
    }

    return self_emmitance + radiance;
  }
                              
  if ( depth < 1 )
    cerr << "\nLigg här och gnag... " << endl;

  return Color(1.f, 0.f, 0.f);//radiance;
}

void Render(vec3 cam_pos,
            const vector<Triangle>& ta,
            const vector<Sphere>& sa,
            const vector<PointLight>& pla,
            const vector<Triangle*> &ala,
            Framebuffer &frame_buffer,
            const float& delta,
            const float& pixel_center_minimum,
            const double& start_time,
            const int& spp) {
  float delta2 = delta - (delta / 2.0f);
  float sppf = static_cast<float>(spp);
  cout << "Rendering!" << endl;
  #pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      Color temp_color = Color(0.0f, 0.0f, 0.0f);
      for (int s = 0; s < spp; s++) {
        float random_x = distribution(generator) * delta2;
        float random_y = distribution(generator) * delta2;

        vec3 pixel_random_point =
            vec3(i * delta + pixel_center_minimum + random_x,
                 j * delta + pixel_center_minimum + random_y, 4.0f);
        const Ray ray(pixel_random_point, pixel_random_point - cam_pos);
        temp_color = temp_color + Raytrace(ray, ta, sa, pla, ala, 0);
      }
      frame_buffer[i][j] = (temp_color / (float)spp);
    }
    float percentage = 100.f*static_cast<float>(i) / (static_cast<float>(WIDTH) - 1.f);
    double elapsed_time = ((std::clock() - start_time) / ((double)CLOCKS_PER_SEC * 60.0));
    double est_total_time = (100.0/(percentage))*elapsed_time;
    double est_time_left = est_total_time - elapsed_time;
    fprintf(stderr, "\r\tProgress:  %1.2f%%, \tEstimated time left:  %.1f min", percentage, est_time_left);
    //fprintf(stderr, "\r\tRender Progress:  %1.2f%%, \tElapsed time:  %.1f min, \tEstimated total time:  %.1f min, \tEstimated time left:  %.1f min", percentage, elapsed_time, est_total_time, est_time_left);
  }
  cout << endl;
}

int main() {
  cout << "GI-Ray to the rescue" << endl;

  std::clock_t start = std::clock();
  #ifdef _OPENMP
    std::cout << "\nUsing multiple CPU cores!" << std::endl;
    double start_time = omp_get_wtime();
  #else
    std::cout << "\nMultithreading not supported" << std::endl;
  #endif

  // Create camera
  vec3 camera_pos = vec3(0.0f, 0.0f, 4.95f);
  vec3 camera_plane[4];
  camera_plane[0] = vec3(-1.0f,-1.0f, 4.0f);
  camera_plane[1] = vec3( 1.0f,-1.0f, 4.0f);
  camera_plane[2] = vec3( 1.0f, 1.0f, 4.0f);
  camera_plane[3] = vec3(-1.0f, 1.0f, 4.0f);

  float delta = (camera_plane[1].x - camera_plane[0].x)/WIDTH; // assumes square pixels
  float pixel_center_minimum = camera_plane[0].x + delta/2;

  Framebuffer frame_buffer(WIDTH, vector<vec3>(HEIGHT));

  ClearColorBuffer(Color(1.0f, 0.2f, 0.9f), frame_buffer);

  vector<vec3>        vertex_array;
  vector<Material>    material_array;
  vector<Triangle>    triangle_array;
  vector<Sphere>      sphere_array;
  vector<PointLight>  point_light_array;
  vector<Triangle*>   area_light_array;

  vertex_array.reserve(4000);
  material_array.reserve(32);
  triangle_array.reserve(4000);
  //point_light_array.reserve();
  area_light_array.reserve(64);
  sphere_array.reserve(8);

  cout << "Creating the scene..." << endl;
  CreateScene(vertex_array, triangle_array, sphere_array, material_array,
              point_light_array, area_light_array);

  Render(camera_pos, triangle_array, sphere_array, point_light_array, area_light_array,
       frame_buffer, delta, pixel_center_minimum, start, SAMPLES);

  double cpu_duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "\nCPU wall time: " << cpu_duration;
  #ifdef _OPENMP
    double duration = omp_get_wtime() - start_time;
    std::cout << "\nTime taken with Multithreading: " << duration << std::endl;
  #endif

  float newGamma = 1.0f;
  string test;
  stringstream ss;
  ss << "results/cornell_" << WIDTH << "x" << HEIGHT << "_" << SAMPLES << "spp_" << MAX_DEPTH << "bounces_";
  while (true) {
    stringstream gs;
    gs << setprecision(2) << GAMMA_FACTOR;
    string filename = ss.str() + gs.str() + "gamma.ppm";
    cout << "Render DONE!\nWriting to file: " << filename << endl;
    SaveImage(filename.c_str(), frame_buffer);
    cout << "Enter new gamma value: " << endl;
    cin >> test;
    newGamma = stof(test);
    if (newGamma <= 0) {
      break;
    }
    GAMMA_FACTOR = newGamma;
  }

  cout << "\nGI-Ray finished without any problems" << endl;
  cout << "\nEXITING GI-RAY..." << endl;

  return 0;
}
