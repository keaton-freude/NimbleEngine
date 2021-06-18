#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

namespace {

template <typename T>
static int __NimbleGetOpenGLType() {
	if constexpr(std::is_same_v<T, float>) {
		return GL_FLOAT;
	} else if constexpr(std::is_same_v<T, int>) {
		return GL_INT;
	}
	return 0;
	// etc...
}

template <typename First, typename... Rest>
static void __DoNimbleSetVertexAttribPointers(int &index, uintptr_t &offset, int stride) {
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index,
						  sizeof(First) / sizeof(First::value_type),
						  __NimbleGetOpenGLType<typename First::value_type>(),
						  GL_FALSE,
						  stride,
						  reinterpret_cast<void *>(offset));

	if constexpr(sizeof...(Rest) > 0) {
		index++;
		offset += sizeof(First);
		__DoNimbleSetVertexAttribPointers<Rest...>(index, offset, stride);
	}
}

template <typename First, typename... Rest>
static int __NimbleGetStride() {
	if constexpr(sizeof...(Rest) == 0) {
		return sizeof(First);
	} else {
		return sizeof(First) + __NimbleGetStride<Rest...>();
	}
}

template <typename First, typename... Rest>
static void __NimbleSetVertexAttribPointers() {
	int index = 0;
	uintptr_t offset = 0;
	__DoNimbleSetVertexAttribPointers<First, Rest...>(index, offset, __NimbleGetStride<First, Rest...>());
}
} // namespace

namespace Nimble {

struct Position {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3);
	}
	glm::vec3 position;

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3>();
	}

	Position() = default;
	Position(float x, float y, float z) : position(x, y, z) {
	}
};

struct PositionColor {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec4);
	}

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3, glm::vec4>();
	}

	glm::vec3 position;
	glm::vec4 color;
	PositionColor() = default;
	PositionColor(glm::vec3 position, glm::vec4 color) : position(position), color(color) {
	}
};

struct PositionUv {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec2);
	}

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3, glm::vec2>();
	}

	glm::vec3 position;
	glm::vec2 uv;
	PositionUv(glm::vec3 position, glm::vec2 uv) : position(position), uv(uv) {
	}
};

struct PositionNormal {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec3);
	}

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3, glm::vec3>();
	}

	glm::vec3 position;
	glm::vec3 normal;
	PositionNormal() = default;
	PositionNormal(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {
	}
};

struct PositionNormalUv {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
	}

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3, glm::vec3, glm::vec2>();
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;

	PositionNormalUv() = default;
	PositionNormalUv(glm::vec3 position, glm::vec3 normal, glm::vec2 texture)
	: position(position), normal(normal), texture(texture) {
	}
};

struct PositionNormalUvTangentBitangent {
	static constexpr size_t SizeInBytes() {
		return sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec3);
	}

	static void SetVertexAttribPointers() {
		__NimbleSetVertexAttribPointers<glm::vec3, glm::vec3, glm::vec2, glm::vec3, glm::vec3>();
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	PositionNormalUvTangentBitangent() = default;
	PositionNormalUvTangentBitangent(glm::vec3 position, glm::vec3 normal, glm::vec2 texture, glm::vec3 tangent, glm::vec3 bitangent)
	: position(position), normal(normal), texture(texture), tangent(tangent), bitangent(bitangent) {
	}
};

} // namespace Nimble