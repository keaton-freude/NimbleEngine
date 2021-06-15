#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace Nimble {

// Represents the bounds of an orthographic projection
// This is a cube, in world-space, which can be used to create
// an orthographic projection
// Beyond rendering, useful for shadow-mapping directional lights
struct OrthoProjection {

	// Restricting the OrthoProjection to having equal-sized values across their dimension
	// So (abs(left) == abs(right)) and (abs(left) + abs(right) == width)
	// Applies to width and height.
	float width = 0.0f;
	float height = 0.0f;
	// Maybe we need to add a bias factor to scoot the near-portion up a bit (or just have a constant bias)
	// in case of any rendering artifacts
	float depth = 0.0f;

	OrthoProjection() = default;

	OrthoProjection(float width, float height, float depth) : width(width), height(height), depth(depth) {
	}

	[[nodiscard]] glm::mat4 CreateProjection() const {
		return glm::ortho(-(width / 2.f), width / 2.f, -(height / 2.f), height / 2.f, 0.0f, depth);
	}
};
} // namespace Nimble