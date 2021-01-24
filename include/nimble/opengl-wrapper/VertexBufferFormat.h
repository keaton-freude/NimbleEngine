#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

namespace Nimble {
struct Position {
	static constexpr size_t SizeInBytes() {
		return sizeof(float) * 3;
	}
	float x;
	float y;
	float z;

	static void SetVertexAttribPointers() {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)Position::SizeInBytes(), (void *)0);
	}

	Position() = default;
	Position(float x, float y, float z) : x(x), y(y), z(z) {
	}
};

struct PositionColor {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec4);
	}

	static void SetVertexAttribPointers() {
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)PositionColor::SizeInBytes(), (void *)0);

		// Color
		// Offset is 12 bytes to account for 3 floats of position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (int)PositionColor::SizeInBytes(), (void *)12);
	}

	glm::vec3 position;
	glm::vec4 color;
	PositionColor(glm::vec3 position, glm::vec4 color) : position(position), color(color) {
	}
};

struct PositionNormal {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec3);
	}

	static void SetVertexAttribPointers() {
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)PositionNormal::SizeInBytes(), (void *)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (int)PositionNormal::SizeInBytes(), (void *)12);
	}

	glm::vec3 position;
	glm::vec3 normal;
	PositionNormal() = default;
	PositionNormal(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {
	}
};

struct PositionNormalUv {
	static constexpr size_t SizeInBytes() {
		//return sizeof(PositionNormalUv);
		return sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
	}

	static void SetVertexAttribPointers() {
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)PositionNormalUv::SizeInBytes(), (void *)0);

		// Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (int)PositionNormalUv::SizeInBytes(), (void *)12);

		// UV
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (int)PositionNormalUv::SizeInBytes(), (void *)24);
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;

	PositionNormalUv() = default;
	PositionNormalUv(glm::vec3 position, glm::vec3 normal, glm::vec2 texture)
	: position(position), normal(normal), texture(texture) {
	}
};

} // namespace Nimble