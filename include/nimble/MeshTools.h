#pragma once

#include "Mesh.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"

namespace Nimble {
class MeshTools {
public:
	// just create a basic triangle, not flexible.. think demo triangle
	static Mesh<Nimble::Position> CreateTriangle() {
		// half the screen in height, half the screen in width
		return Mesh<Nimble::Position>({ { 0.0f, 0.5f, 0.0f }, // top
										{ -.5f, -.5f, 0.0f },
										{ .5f, -.5f, 0.0f }

									  },
									  { { 0, 1, 2 } },
									  1,
									  Mesh<Nimble::Position>::CreateVao());
	}

	// Remove me eventually, just for testing color shader
	static Mesh<Nimble::PositionColor> CreateColoredTriangle() {
		// Same as the demo triangle, but with some colors
		glm::vec3 vert1Pos = glm::vec3({ 0.0f, 0.5f, 0.0f });
		glm::vec3 vert2Pos = glm::vec3({ -.5f, -.5f, 0.0f });
		glm::vec3 vert3Pos = glm::vec3({ 0.5f, -.5f, 0.0f });

		glm::vec4 vert1Color = glm::vec4({ 1.0f, 0, 0, 1.0f });
		glm::vec4 vert2Color = glm::vec4({ 0, 1.0f, 0, 1.0f });
		glm::vec4 vert3Color = glm::vec4({ 0, 0, 1.0f, 1.0f });

		return Mesh<Nimble::PositionColor>({ { vert1Pos, vert1Color }, { vert2Pos, vert2Color }, { vert3Pos, vert3Color } },
										   { { 0, 1, 2 } },
										   1,
										   Mesh<Nimble::PositionColor>::CreateVao());
	}

	static Mesh<Nimble::PositionColor> CreateColoredPlane() {
		glm::vec3 vert1Pos = glm::vec3({ -1.0f, 1.0f, 0.0f });
		glm::vec3 vert2Pos = glm::vec3({ -1.0f, -1.0f, 0.0f });
		glm::vec3 vert3Pos = glm::vec3({ 1.0f, -1.0f, 0.0f });
		glm::vec3 vert4Pos = glm::vec3({ 1.0f, 1.0f, 0.0f });

		glm::vec4 vert1Color = glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f });
		glm::vec4 vert2Color = glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f });
		glm::vec4 vert3Color = glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f });
		glm::vec4 vert4Color = glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f });

		return Mesh<Nimble::PositionColor>({ { vert1Pos, vert1Color }, { vert2Pos, vert2Color }, { vert3Pos, vert3Color }, { vert4Pos, vert4Color } },
										   { { 0, 1, 2, 0, 2, 3 } },
										   2,
										   Mesh<Nimble::PositionColor>::CreateVao());
	}

	static Mesh<Nimble::PositionUv> CreateTexturedPlane() {
		glm::vec3 vert1Pos = glm::vec3({ -1.0f, 1.0f, 0.0f });
		glm::vec3 vert2Pos = glm::vec3({ -1.0f, -1.0f, 0.0f });
		glm::vec3 vert3Pos = glm::vec3({ 1.0f, -1.0f, 0.0f });
		glm::vec3 vert4Pos = glm::vec3({ 1.0f, 1.0f, 0.0f });

		glm::vec2 vert1Uv = glm::vec2({ 0.0f, 1.0f });
		glm::vec2 vert2Uv = glm::vec2({ 0.0f, 0.0f });
		glm::vec2 vert3Uv = glm::vec2({ 1.0f, 0.0f });
		glm::vec2 vert4Uv = glm::vec2({ 1.0f, 1.0f });

		return Mesh<Nimble::PositionUv>({ { vert1Pos, vert1Uv }, { vert2Pos, vert2Uv }, { vert3Pos, vert3Uv }, { vert4Pos, vert4Uv } },
										{ { 0, 1, 2, 0, 2, 3 } },
										2,
										Mesh<Nimble::PositionUv>::CreateVao());
	}
};
} // namespace Nimble