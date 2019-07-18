#include "nimble/camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "nimble/input/InputManager.h"

using namespace Nimble;

Camera::Camera()
: _focusPoint(glm::vec3(0.f, 0.f, 0.f)), _rotation(0.f, 0.f), _distance(0.f), _position(1.0f) {
}

Camera::Camera(glm::vec3 focusPoint)
: _focusPoint(focusPoint), _rotation(0.f, 0.f), _distance(0.f), _position(1.0f) {
}

glm::mat4 Camera::GetView() const {
	return (glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -_distance)) * _position) * glm::mat4(1.0f);
}

void Camera::Rotate(float amount, glm::vec3 axis) {
	_position = _position * glm::rotate(glm::mat4(1.0f), amount, axis);
}

void Camera::Update(const Time &time) {
	// Normalized vector with movement
	auto mouse = Input::Get().GetMouseMovement();

	spdlog::info("Mouse (X,Y): {},{}", mouse.x, mouse.y);

	// Rotate around the X & Y
	// For X, we rotate around the mouse's Y coordinate
	Rotate(mouse.y * 0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
	// For Y, we rotate around the mouse's X coordinate
	Rotate(mouse.x * 0.005f, glm::vec3(0.0f, 1.0f, 0.0f));
}