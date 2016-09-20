#include <iostream>
#include <glm/glm.hpp>
#include "commons.h"
#include "camera.h"
#include "ray.h"
#include "scene.h"
#include <ctime>

int main() {
  std::clock_t start = std::clock();

  Scene scene = Scene();
  Camera cam = Camera(Vertex(-2,0,0), Vertex(-1,0,0), Direction(1,0,0), Direction(0,0,1));
  cam.ClearColorBuffer(glm::vec3(155,45,90));

  //cam.Render(scene);
  //cam.CreateImage("max_intensity_ep1",true);
  //cam.CreateImage("sqrt_intensity_ep1",false);

  cam.ChangeEyePos();

  cam.Render(scene);
  cam.CreateImage("max_intensity_ep2",true);
 // cam.CreateImage("sqrt_intensity_ep2",false);

  double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "Execution time: " << duration << std::endl;
}
