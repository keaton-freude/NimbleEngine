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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "nimble/engine/Time.h"

namespace Nimble {

class ICamera {
public:
	virtual ~ICamera() = default;

	// Camera implementations must provide a method which returns
	// the view matrix, suitable for MVP calculations in-shader
	virtual glm::mat4 GetView() = 0;

	// Hook where cameras can update their internal state, ultimately mutating
	// their view
	virtual void Update(const Time& time) = 0;

	virtual glm::vec3 GetPosition() = 0;
};

} // namespace Nimble