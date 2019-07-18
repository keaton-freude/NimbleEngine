#include "nimble/camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "nimble/input/InputManager.h"

using namespace Nimble;

Camera::Camera() : _focusPoint(glm::vec3(0.f, 0.f, 0.f)), _rotation(0.f, 0.f), _position(1.0f) {
	_position = glm::vec3(0.0f, 0.0f, -2.0f);
}

Camera::Camera(glm::vec3 focusPoint)
: _focusPoint(focusPoint), _rotation(0.f, 0.f), _position(1.0f) {
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(_position, _focusPoint, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Rotate(glm::vec2 delta) {
	const float RotateSpeed = 3.0f;
	delta *= RotateSpeed;
	_rotation += delta * RotateSpeed;
	glm::vec3 camFocusVector = _position - _focusPoint;
	glm::vec3 direction = glm::normalize(camFocusVector);

	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
	glm::vec3 cameraUp = glm::normalize(glm::cross(direction, right));

	spdlog::info("Camera up: {},{},{}", cameraUp.x, cameraUp.y, cameraUp.z);


	glm::mat4 upRotation = glm::rotate(-delta.x, cameraUp);
	glm::mat4 rightRotation = glm::rotate(-delta.y, right);

	glm::quat upQuat = glm::quat_cast(upRotation);
	glm::quat rightQuat = glm::quat_cast(rightRotation);

	camFocusVector = upQuat * camFocusVector;
	camFocusVector = rightQuat * camFocusVector;

	auto newPosition = camFocusVector + _focusPoint;
	_position = newPosition;
}

void Camera::Update(const Time &time) {
	const float RotateSpeed = 2.0f;
	// Normalized vector with movement
	auto mouse = Input::Get().GetMouseMovement();

	/*if(glm::abs(mouse.y) > 0.01f) {
		spdlog::info("Mouse (X,Y): {},{}", mouse.x, mouse.y);
	}*/

	_rotation.y += mouse.y * time.dt() * RotateSpeed;
	_rotation.x += mouse.x * time.dt() * RotateSpeed;
}