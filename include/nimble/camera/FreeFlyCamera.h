//
// Created by freudek on 1/31/21.
//

#pragma once

#include "nimble/camera/Camera.h"

namespace Nimble {

class FreeFlyCamera : public ICamera {
private:
	glm::vec3 _position{0.0f, 5.0f, 3.0f};
	glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
	glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
	float yaw{0.f};
	float pitch{0.f};
	float roll{0.f};
	float _moveSpeed = 15.0f;
	float _rotateSpeed = 2.5f;
public:
	FreeFlyCamera();
	FreeFlyCamera(float rotateSpeed);
	virtual ~FreeFlyCamera() = default;

	glm::mat4 GetView() override;
	void Update(const Time& time) override;
	glm::vec3 GetPosition() override;
};

}
