#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace Nimble {

// Represents the bounds of an orthographic projection
// This is a cube, in world-space, which can be used to create
// an orthographic projection
// Beyond rendering, useful for shadow-mapping directional lights
struct OrthoProjection {

	// Wow. MSVC apparently used to use `near` and `far` as keywords, so we can't use
	// them so adding underscores to everything...
	float _right = 0.0f;
	float _left = 0.0f;
	float _top = 0.0f;
	float _bottom = 0.0f;
	float _near = 0.0f;
	float _far = 0.0f;

	OrthoProjection() = default;

	OrthoProjection(float right, float left, float top, float bottom, float near, float far)
	: _right(right), _left(left), _top(top), _bottom(bottom), _near(near), _far(far) {}

	[[nodiscard]] glm::mat4 CreateProjection() const {
		return glm::ortho(_left, _right, _bottom, _top, _near, _far);
	}
};
} // namespace Nimble