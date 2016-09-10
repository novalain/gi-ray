#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <string>

#define WIDTH 1000
#define HEIGHT 1000

class Camera
{
private:
    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 up_vector_;

    float focal_length_;
    float fov_; // field of view

    glm::vec3 framebuffer[ WIDTH * HEIGHT ];

    //TODO: aspect ratio?
    //float aspect_ratio_;

    void init();

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up_vector);

    void ExportToBMP(std::string filename);

    int get_width() { return WIDTH; }
    int get_height() { return HEIGHT; }
    float get_fov() { return fov_; }
    float get_focal_length() { return focal_length_; }
    glm::vec3 get_position() { return position_; }
    glm::vec3 get_direction() { return direction_; }
    glm::vec3 get_up_vector() { return up_vector_; }

    void set_focal_length(float f) { focal_length_ = f; }
    void set_fov(float fov) { fov_ = fov; }
    void set_position(glm::vec3 pos) { position_ = pos; }
    void set_direction(glm::vec3 dir) { direction_ = dir; }
    void set_up_vector(glm::vec3 up_vec) { up_vector_ = up_vec; }
};

#endif // CAMERA_H
