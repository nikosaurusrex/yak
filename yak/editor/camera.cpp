#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(f32 fov, f32 aspect, f32 near, f32 far)
    : fov(fov), aspect(aspect), near(near), far(far) {

    yaw = 0;
    pitch = 0;
    focal_point = glm::vec3(0.0f);
    distance = 5;
}

void Camera::resize(s32 width, s32 height) {
    aspect = (f32)width / (f32)height;  

    projection = glm::perspective(glm::radians(fov), aspect, near, far);

    glm::quat orientation = glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
    auto forward = glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));
    auto position = focal_point - forward * distance;

    view = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);;
    view = glm::inverse(view); 
}
