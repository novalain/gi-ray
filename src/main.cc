#include <iostream>
#include <glm/glm.hpp>
#include "commons.h"
#include "camera.h"
#include "scene.h"
#include "material.h"
#ifdef _OPENMP
  #include <omp.h>
#endif
#include <ctime>

int main() {
  std::clock_t start = std::clock();
  #ifdef _OPENMP
    double start_time = omp_get_wtime();
  #endif

  Scene scene = Scene();
  Camera cam = Camera(Vertex(-2,0,0), Vertex(-1,0,0), Direction(1,0,0), Direction(0,0,1));
  cam.ClearColorBuffer(glm::vec3(155,45,90));

  //cam.Render(scene);
  //cam.CreateImage("max_intensity_ep1",true);
  //cam.CreateImage("sqrt_intensity_ep1",false);

  cam.ChangeEyePos();

  cam.Render(scene, 4);
  cam.CreateImage("max_intensity_ep2", true);
  cam.CreateImage("sqrt_intensity_ep2", false);

  double cpu_duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "\nExecution time across all cores: " << cpu_duration;
  #ifdef _OPENMP
    double duration = omp_get_wtime() - start_time;
    std::cout << "\nReal time taken: " << duration << std::endl;
  #else
    std::cout << "\nMultithreading not supported" << std::endl;
  #endif

}
