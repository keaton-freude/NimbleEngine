#pragma once

#include "glm/glm.hpp"
#include "glm/vec3.hpp"

namespace Nimble {
struct Position {
	enum { SizeInBytes = sizeof(float) * 3 };
	float x;
	float y;
	float z;

	Position() = default;
	Position(float x, float y, float z) : x(x), y(y), z(z) {
	}
};

struct PositionColor {
	// 3 floats of position, 4 floats of RGBA
	enum { SizeInBytes = sizeof(float) * 3 + sizeof(float) * 4 };
	glm::vec3 position;
	glm::vec4 color;
	PositionColor(glm::vec3 position, glm::vec4 color) : position(position), color(color) {
	}
};
} // namespace Nimble