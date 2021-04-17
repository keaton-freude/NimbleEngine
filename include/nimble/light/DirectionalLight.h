#pragma once

/**
 * For now we'll model a Directional Light simply as a direction
 * + color. In the future, for PBR we'll probably want to include
 * an intensity factor
 */

#include <glm/glm.hpp>

namespace Nimble {
struct DirectionalLight {
	glm::vec3 direction;
	// Do we care about alpha?
	glm::vec3 color;

	// Whether this light is currently turned on
	bool enabled;

	DirectionalLight() : direction(0.f, 0.f, 0.f), color(0.f, 0.f, 0.f), enabled(false) {
		// Due to no good directional light defaults, we'll treat it as off
		// by default. User should set their light config, and enable the light
	}

	DirectionalLight(glm::vec3 direction, glm::vec3 color) : direction(direction), color(color) {
	}
};
} // namespace Nimble