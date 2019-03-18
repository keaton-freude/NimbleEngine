#pragma once

#include "Mesh.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"

class MeshTools {
public:
	// just create a basic triangle, not flexible.. think demo triangle
	static Mesh<Nimble::Position> CreateTriangle() {
		// half the screen in height, half the screen in width
		return Mesh<Nimble::Position>({ { 0.0f, 0.5f, 0.0f }, // top
										{ -.5f, -.5f, 0.0f },
										{ .5f, -.5f, 0.0f }

									  },
									  { { 0, 1, 2 } });
	}
};