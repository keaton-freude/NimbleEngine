#pragma once

#include "nimble/camera/Camera.h"

namespace Nimble {

class FixedPointOrbitCamera : public ICamera {
	// This is the location, in world-space, that we will be focusing on
	glm::vec3 _focusPoint;
	// This is our current rotation around the Focus Point
	// where x is the rotation around the X axis (up & around, or down and around)
	// and y is the rotation around the Y axis (left -> right, or right -> left movement)
	glm::vec2 _rotation;

	glm::vec3 _position;
	glm::vec3 _rotatedPosition;
	float _rotateSpeed;

	glm::quat _quatRotation;

public:
	// assume FocusPoint is at 0, 0, 0
	FixedPointOrbitCamera();
	virtual ~FixedPointOrbitCamera() = default;
	FixedPointOrbitCamera(glm::vec3 focusPoint, float rotateSpeed);

	glm::mat4 GetView() override;

	void SetPosition(glm::vec3 position) {
		_position = position;
	}

	void Update(const Time &time) override;

	glm::vec3 Right() const;
	glm::vec3 Up() const;

private:
};

}