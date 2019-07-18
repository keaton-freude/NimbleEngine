// For now, not supporting extra types of Camera's, just a simple Orbit Camera

// An orbit camera is fixated on a specific point, typically (0, 0, 0) and allows the user
// to rotate around this specific point, like an object in orbit

// We'll allow the creator of the camera to set the focus point, and allow rotating
// around the object holding right-click and dragging left-right/up-down

// The camera will also provide a View matrix. The render loop will need to call
// Update on the camera, then retrieve the View matrix. This matrix can then be used
// in MVP matrix calculations per-object

#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "nimble/engine/Time.h"

namespace Nimble {
class Camera {
	// This is the location, in world-space, that we will be focusing on
	glm::vec3 _focusPoint;
	// This is our current rotation around the Focus Point
	// where x is the rotation around the X axis (up & around, or down and around)
	// and y is the rotation around the Y axis (left -> right, or right -> left movement)
	glm::vec2 _rotation;

	//// The position, in world-space of our camera
	// glm::vec3 _position;
	float _distance;
	glm::mat4 _position;

public:
	// assume FocusPoint is at 0, 0, 0
	Camera();
	Camera(glm::vec3 focusPoint);

	glm::mat4 GetView() const;

	void SetDistance(float distance) {
		_distance = distance;
	}

	void Rotate(float amount, glm::vec3 axis);

	void Update(const Time &time);

private:
};
} // namespace Nimble