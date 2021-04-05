#include "nimble/camera/FreeFlyCamera.h"
#include "nimble/input/InputManager.h"

#include <glm/glm.hpp>

using namespace Nimble;

FreeFlyCamera::FreeFlyCamera() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront =  glm::normalize(direction);
}

FreeFlyCamera::FreeFlyCamera(float rotateSpeed) {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront =  glm::normalize(direction);
}

glm::mat4 FreeFlyCamera::GetView() {
	return glm::lookAt(_position, _position + cameraFront, cameraUp);
}

glm::vec3 Damp(glm::vec3 source, glm::vec3 target, float smoothing, float dt) {
	return glm::mix(source, target, 1.0f - pow(smoothing, dt));
}

float lerp(float v0, float v1, float t) {
	return (1 - t) * v0 + t * v1;
}

void FreeFlyCamera::Update(const Time &time) {

	if (Input::Get().IsMouseRightDown()) {
		auto mouseDelta = Input::Get().GetMouseMovement();
		targetYaw += mouseDelta.x * _rotateSpeed * time.dt();
		targetPitch -= mouseDelta.y * _rotateSpeed * time.dt();
	}

	yaw = lerp(yaw, targetYaw, time.dt() * 20.0f);
	pitch = lerp(pitch, targetPitch, time.dt() * 20.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront =  glm::normalize(direction);

	//_rotation = Damp(_rotation, _targetRotation, 0.0001f, time.dt());

	if (Input::Get().IsKeyPressed("camera_left")) {
		_position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * _moveSpeed * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_right")) {
		_position += glm::normalize(glm::cross(cameraFront, cameraUp)) * _moveSpeed * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_forward")) {
		_position += _moveSpeed * cameraFront * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_backward")) {
		_position -= _moveSpeed * cameraFront * time.dt();
	}
}

glm::vec3 FreeFlyCamera::GetPosition() {
	return _position;
}
