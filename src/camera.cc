#include "camera.h"

Camera::Camera() {
  init(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1));
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector) {
  init(position, direction, up_vector);
}

void Camera::init(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector) {
  position_ = position;
  direction_ = direction;
  up_vector_ = up_vector;

  focal_length_ = 1;
  fov_ = 90;
}

void Camera::ExportToBMP(std::string filename) {

}
