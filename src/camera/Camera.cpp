#include "nimble/camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Nimble;

Camera::Camera()
: _focusPoint(glm::vec3(0.f, 0.f, 0.f)), _rotation(0.f, 0.f), _position(0.f, 0.f, 0.f) {
}

Camera::Camera(glm::vec3 focusPoint)
: _focusPoint(focusPoint), _rotation(0.f, 0.f), _position(0.f, 0.f, 0.f) {
}

glm::mat4 Camera::GetView() const {
	glm::vec3 cameraDirection = glm::normalize(_position - _focusPoint);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	return glm::lookAt(_position, _focusPoint, cameraUp);
}