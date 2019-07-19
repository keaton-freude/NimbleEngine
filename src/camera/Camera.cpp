#include "nimble/camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "nimble/input/InputManager.h"

using namespace Nimble;

Camera::Camera()
: _focusPoint(glm::vec3(0.f, 0.f, 0.f)), _rotation(0.f, 0.f), _position(0.0f, 0.0f, -3.0f),
  _rotateSpeed(1.0f) {
	_position = glm::vec3(0.0f, 0.0f, -2.0f);
}

Camera::Camera(glm::vec3 focusPoint, float rotateSpeed)
: _focusPoint(focusPoint), _rotation(0.f, 0.f), _position(0.0f, 0.0f, -3.0f), _rotateSpeed(rotateSpeed) {
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(_position, _focusPoint, Up());
}

glm::vec3 Camera::Right() const {
	glm::vec3 camFocusVector = _position - _focusPoint;
	glm::vec3 direction = glm::normalize(camFocusVector);
	return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
}

glm::vec3 Camera::Up() const {
	auto right = Right();
	glm::vec3 camFocusVector = _position - _focusPoint;
	glm::vec3 direction = glm::normalize(camFocusVector);
	return glm::normalize(glm::cross(direction, right));
}

void Camera::Rotate(glm::vec2 delta) {
	delta *= _rotateSpeed;
	if(_rotation.y + delta.y > 0.9f || _rotation.y + delta.y < -1.8f) {
		return;
	}
	_rotation += delta;
	auto camFocusVector = _position - _focusPoint;

	// Create rotations around our up and right vectors
	glm::mat4 upRotation = glm::rotate(-delta.x, Up());
	glm::mat4 rightRotation = glm::rotate(-delta.y, Right());

	// Convert the matrices to quaternions
	glm::quat upQuat = glm::quat_cast(upRotation);
	glm::quat rightQuat = glm::quat_cast(rightRotation);

	// Apply the rotations to our camera
	camFocusVector = upQuat * camFocusVector;
	camFocusVector = rightQuat * camFocusVector;

	// Officially set the position
	_position = camFocusVector + _focusPoint;
}

void Camera::Update(const Time &time) {
	// Normalized vector with movement
	auto mouse = Input::Get().GetMouseMovement();

	_rotation.y += mouse.y * time.dt() * _rotateSpeed;
	_rotation.x += mouse.x * time.dt() * _rotateSpeed;
}