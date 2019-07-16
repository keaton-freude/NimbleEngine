#pragma once

#include "nimble/opengl-wrapper/Buffer.h"
#include <bits/stdint-uintn.h>
#include <stddef.h>
#include <vector>

namespace Nimble {


class IndexBuffer {
public:
	typedef std::vector<uint32_t> ElementStorage;

private:
	// opengl EBO handle
	uint32_t _elementBufferHandle;
	ElementStorage _elements;
	BufferUsageType _usageType;

public:
	IndexBuffer() = default;
	IndexBuffer(size_t numElements, BufferUsageType usage);

	// Set the
	void SetData(const ElementStorage &data);
	void Bind();

	size_t GetNumIndices() {
		return _elements.size();
	}
};

} // namespace Nimble
