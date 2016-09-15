#include <iostream>
#include <glm/glm.hpp>
#include "camera.h"
#include "commons.h"
#include "triangle.h"
#include "ray.h"
#include "scene.h"

int main() {
  std::cout << "Hello World" << std::endl;
  Camera cam;
  Scene scene;

  cam.ClearColorBuffer(glm::vec3(155,45,90));
  cam.CreateImage("",true);
}
