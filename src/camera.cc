#include "camera.h"

Camera::Camera() {
    init();
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector) {
    init();
    position_ = position;
    direction_ = direction;
    up_vector_ = up_vector;
}

void Camera::ExportToBMP(std::string filename) {

}

void Camera::init() {
    position_ = glm::vec3(0,0,0);
    direction_ = glm::vec3(0,1,0);
    up_vector_ = glm::vec3(0,0,1);

    width_ = 32;
    height_ = 32;

    focal_length_ = 1;
    fov_ = 90;
}
