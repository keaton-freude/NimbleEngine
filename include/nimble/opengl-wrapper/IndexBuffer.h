#pragma once

#include "nimble/IMesh.h"
#include "nimble/opengl-wrapper/Buffer.h"
#include <stdint.h>
#include <stddef.h>
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
	IndexBuffer(IMesh *mesh, BufferUsageType usage);

	size_t GetNumFaces() const {
		return _numFaces;
	}

	// Set the
	// void SetData(const ElementStorage &data);
	void Bind();
};

} // namespace Nimble
