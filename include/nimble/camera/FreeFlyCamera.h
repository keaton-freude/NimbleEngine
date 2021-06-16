//
// Created by freudek on 1/31/21.
//

#pragma once

#include "nimble/camera/Camera.h"

namespace Nimble {

class FreeFlyCamera : public ICamera {
private:
	glm::vec3 _position{ -25.f, 23.f, -20.8f };
	glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
	glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

	// The current rotation values
	float yaw{ 0.f };
	float pitch{ 0.f };
	float roll{ 0.f };

	// The values we are interpolating towards for smooth movement
	float targetYaw{ 392.f };
	float targetPitch{ 328.f };
	float targetRoll{ 0.f };

	float _moveSpeed = 15.0f;
	float _rotateSpeed = 300.f;

public:
	FreeFlyCamera();
	FreeFlyCamera(float rotateSpeed);
	virtual ~FreeFlyCamera() = default;

	glm::mat4 GetView() override;
	void Update(const Time &time) override;
	glm::vec3 GetPosition() override;

	float GetRotateSpeed() const;
	void SetRotateSpeed(float rotateSpeed);
};

} // namespace Nimble
