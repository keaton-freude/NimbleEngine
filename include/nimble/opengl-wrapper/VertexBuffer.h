#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <stdint.h>
#include <vector>

#include "nimble/IMesh.h"
#include "nimble/opengl-wrapper/Buffer.h"

namespace Nimble {

class VertexBuffer {
public:
private:
	uint32_t _vertexBufferHandle;
	BufferUsageType _usageType;

public:
	// TODO: Consider deleting defaults? Are they useful?
	VertexBuffer() = default;

	VertexBuffer(BufferUsageType usageType) {
		glGenBuffers(1, &_vertexBufferHandle);
		_usageType = usageType;
	}

	void LoadFromMesh(const IMesh *mesh) {
		Bind();
		auto glUsageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, (long)mesh->GetNumBytes(), mesh->GetData(), (GLenum)glUsageType);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
	}

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


}; // class VertexBuffer

} // namespace Nimble
