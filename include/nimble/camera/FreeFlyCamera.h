//
// Created by freudek on 1/31/21.
//

#pragma once

#include "nimble/camera/Camera.h"

namespace Nimble {

class FreeFlyCamera : public ICamera {
private:
	glm::vec3 _position{0.0f, -5.0f, -10.0f};
	// pitch, yaw, roll
	glm::vec3 _rotation{};
	glm::vec3 _targetRotation{};
	float _rotateSpeed = 1.0f;
	float _moveSpeed = 5.0f;
public:
	FreeFlyCamera() = default;
	FreeFlyCamera(float rotateSpeed);
	virtual ~FreeFlyCamera() = default;

	glm::mat4 GetView() override;
	void Update(const Time& time) override;
};

}
