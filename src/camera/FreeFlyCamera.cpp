#include "nimble/camera/FreeFlyCamera.h"
#include "nimble/input/InputManager.h"

#include <glm/glm.hpp>

using namespace Nimble;

glm::mat4 FreeFlyCamera::GetView() {

	auto view = glm::translate(-_position);
	// Yaw
	view *= glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	// Pitch
	view *= glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	// Roll
	view *= glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	//view *= glm::translate(_position);

	return glm::inverse(view);
}

glm::vec3 Damp(glm::vec3 source, glm::vec3 target, float smoothing, float dt) {
	return glm::mix(source, target, 1.0f - pow(smoothing, dt));
}

void FreeFlyCamera::Update(const Time &time) {

	if (Input::Get().IsMouseRightDown()) {
		auto mouseDelta = Input::Get().GetMouseMovement();

		_targetRotation.x -= mouseDelta.y * time.dt();
		_targetRotation.y -= mouseDelta.x * time.dt();
	}

	_rotation = Damp(_rotation, _targetRotation, 0.0001f, time.dt());

	if (Input::Get().IsKeyPressed("camera_left")) {
		_position.x += (_moveSpeed) * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_right")) {
		_position.x -= (_moveSpeed) * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_forward")) {
		_position.z += (_moveSpeed) * time.dt();
	}

	if (Input::Get().IsKeyPressed("camera_backward")) {
		_position.z -= (_moveSpeed) * time.dt();
	}
}