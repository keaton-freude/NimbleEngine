#pragma once

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

struct Position2DTexture2D {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec2) + sizeof(glm::vec2);
	}

	static void SetVertexAttribPointers() {
		// Save a call to glVertexAttribPointer by making 4 floats unpack into 2 vec2s in shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (int)Position2DTexture2D::SizeInBytes(), (void *)0);
	}

	glm::vec2 position;
	glm::vec2 texture;

	Position2DTexture2D(glm::vec2 position, glm::vec2 texture)
	: position(position), texture(texture) {
	}
};

} // namespace Nimble