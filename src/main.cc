#include <iostream>
#include <glm/glm.hpp>
#include "camera.h"
#include "vertex.h"
#include "triangle.h"
#include "ray.h"
#include "scene.h"

#define IMG_WIDTH 256
#define IMG_HEIGHT 256
#define CHANNELS 3

int main() {
  std::cout << "Hello World" << std::endl;
  Camera cam;
  Scene scene;

  cam.ClearColorBuffer(glm::vec3(155,45,90));
  cam.CreateImage("",true);
}
