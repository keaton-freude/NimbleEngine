#pragma once

#include "nimble/IMesh.h"
#include "nimble/opengl-wrapper/Buffer.h"
#include "GL/glew.h"

#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace Nimble {


class IndexBuffer {
public:
private:
	// opengl EBO handle
	uint32_t _elementBufferHandle;
	BufferUsageType _usageType;
	size_t _numFaces;

public:
	IndexBuffer() = default;
	IndexBuffer(BufferUsageType usage);

	void LoadFromMesh(IMesh* mesh);

	size_t GetNumFaces() const {
		return _numFaces;
	}

	// Set the
	// void SetData(const ElementStorage &data);
	void Bind();

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

} // namespace Nimble
