#include <iostream>
#include <glm/glm.hpp>
#include "camera.h"
#include "utils/file_utils.h"

#define IMG_WIDTH 256
#define IMG_HEIGHT 256
#define CHANNELS 3

/* Save a random generated image */
void saveDummyImage() {
  float arr[IMG_WIDTH][IMG_HEIGHT][CHANNELS];

  for(int i = 0; i < IMG_WIDTH; i++) {
    for(int j = 0; j < IMG_HEIGHT; j++) {
      for(int k = 0; k < CHANNELS; k++) {
        int value = std::rand() % (int(255) + 1);
        assert(value <= 255 && value >= 0);
        arr[i][j][k] = value;
      }
    }
  }

  FileUtils::SaveImage("results/img.png", IMG_WIDTH, IMG_HEIGHT, arr);
}

int main() {
  std::cout << "Hello World" << std::endl;
  Camera cam;
  saveDummyImage();
}
