#include "nimble/camera/FixedPointOrbitCamera.h"
#include "nimble/input/InputManager.h"

using namespace Nimble;

FixedPointOrbitCamera::FixedPointOrbitCamera()
: _focusPoint(glm::vec3(0.f, 0.f, 0.f)), _rotation(0.f, 0.f), _position(0.0f, 0.0f, -3.0f), _rotateSpeed(1.0f) {
	_position = glm::vec3(0.0f, 0.0f, -2.0f);
}

FixedPointOrbitCamera::FixedPointOrbitCamera(glm::vec3 focusPoint, float rotateSpeed)
: _focusPoint(focusPoint), _rotation(0.f, 0.f), _position(0.0f, 0.0f, -3.0f), _rotateSpeed(rotateSpeed) {
	_quatRotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
}

glm::mat4 FixedPointOrbitCamera::GetView() {
	return glm::translate(glm::mat4_cast(_quatRotation), _rotatedPosition);
}

glm::vec3 FixedPointOrbitCamera::Right() const {
	glm::vec3 camFocusVector = _position - _focusPoint;
	glm::vec3 direction = glm::normalize(camFocusVector);
	return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
}

glm::vec3 FixedPointOrbitCamera::Up() const {
	auto right = Right();
	glm::vec3 camFocusVector = _position - _focusPoint;
	glm::vec3 direction = glm::normalize(camFocusVector);
	return glm::normalize(glm::cross(direction, right));
}

void FixedPointOrbitCamera::Update(const Time &time) {
	// Only update our rotation if the right mouse button is being held
	if(Input::Get().IsMouseRightDown()) {
		// Normalized vector with movement
		auto mouse = Input::Get().GetMouseMovement();

		mouse = mouse * _rotateSpeed;

		glm::quat key_quat = glm::quat(glm::vec3(mouse.y, mouse.x, 0.0f)) * time.dt();

		_quatRotation = key_quat * _quatRotation;
		_quatRotation = glm::normalize(_quatRotation);
	}

	if(Input::Get().IsScrollForward()) {
		const float ZoomSpeed = 0.5f;
		// Move toward the focus point, always maintain a minimum distance
		auto direction = glm::normalize(_focusPoint - _position);

		_position += (direction * ZoomSpeed);

		if(_position.z > -1.0f) {
			_position.z = -1.0f;
		}
	}

	if(Input::Get().IsScrollBackward()) {
		const float ZoomSpeed = 0.5f;
		// Move toward the focus point, always maintain a minimum distance
		auto direction = glm::normalize(_focusPoint - _position);

		_position -= (direction * ZoomSpeed);
	}

	// Zoom in by moving our position away from the focus point, or toward it
	// With a minimum required distance (to avoid clipping through)


	auto camFocusVector = _position - _focusPoint;

	ImGui::Text("Camera Rotation: %f, %f, %f, %f", _quatRotation.w, _quatRotation.x, _quatRotation.y, _quatRotation.z);

	// Apply rotation
	camFocusVector = camFocusVector * _quatRotation;

	// Move camera back
	_rotatedPosition = camFocusVector + _focusPoint;
}
