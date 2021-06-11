#pragma once

/**
 * For now we'll model a Directional Light simply as a direction
 * + color. In the future, for PBR we'll probably want to include
 * an intensity factor
 */

#include "nimble/core/OrthographicProjection.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Nimble {
struct DirectionalLight {
	glm::vec3 direction{};
	// Do we care about alpha?
	glm::vec3 color{};
	// Typically position does not matter, however it is important when creating
	// a shadow frustum. The frustum is limited by resolution (vram), so the frustum
	// needs to translated throughout the scene for coverage
	glm::vec3 position{};
	// Projection parameters for this light, again used for shadow mapping
	OrthoProjection projection{};
	constexpr static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	// Whether this light is currently turned on
	bool enabled = false;

	DirectionalLight() = default;

	DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 position, OrthoProjection projection)
	: direction(direction), color(color), position(position), projection(projection) {
	}

	[[nodiscard]] glm::mat4 GetLightView() const {
		return glm::lookAt(position, position + direction, up);
	}

	[[nodiscard]] glm::mat4 GetLightProjection() const {
		return projection.CreateProjection();
	}

	[[nodiscard]] glm::mat4 GetLightSpaceMatrix() const {
		return GetLightProjection() * GetLightView();
	}
};
} // namespace Nimble