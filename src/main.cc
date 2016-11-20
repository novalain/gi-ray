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
#include <string>

int main() {
  std::cout << "GI-Ray to the rescue" << std::endl;
  std::cout << "\nHow many samples/pixel do you want? ";
  int spp;
  std::cin >> spp;

  while (spp > 0) {
    std::clock_t start = std::clock();

#ifdef _OPENMP
    double start_time = omp_get_wtime();
#else
    std::cout << "\nMultithreading not supported" << std::endl;
#endif

    std::cout << "\tCreating scene and camera..." << std::endl;
    Scene scene = Scene();
    Camera cam = Camera(Vertex(-2, 0, 0), Vertex(-1, 0, 0), Direction(1, 0, 0), Direction(0, 0, 1));
    cam.ClearColorBuffer(glm::vec3(155, 45, 90));

    //cam.Render(scene);
    //cam.CreateImage("max_intensity_ep1",true);
    //cam.CreateImage("sqrt_intensity_ep1",false);

    cam.ChangeEyePos();

    std::cout << "\tRendering scene with " << spp << " samples/pixel..." << std::endl;
    cam.Render(scene, spp);
    std::cout << "\n\tRendering Finished" << std::endl;

    std::string suffix = std::to_string(spp) + "spp";

    std::cout << "\tCreating max intensity image..." << std::endl;
    cam.CreateImage("mi_" + suffix, true);

    std::cout << "\tCreating gamma corrected image..." << std::endl;
    cam.CreateImage("si_" + suffix, false);

    double cpu_duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "\nExecution time across all cores: " << cpu_duration;
#ifdef _OPENMP
    double duration = omp_get_wtime() - start_time;
    std::cout << "\nReal time taken: " << duration << std::endl;
#endif

    std::cout << "\nTo run again specify how many samples/pixel you want (enter '0' to quit): ";
    std::cin >> spp;
  }

  std::cout << "\nGI-Ray finished without any problems" << std::endl;
  std::cout << "\nEXITING GI-RAY..." << std::endl;

  return 0;
}
