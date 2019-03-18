#pragma once

#include <vector>

namespace Nimble {

enum class IndexBufferUsageType { Static, Dynamic };

class IndexBuffer {
public:
	typedef std::vector<uint32_t> ElementStorage;

private:
	// opengl EBO handle
	uint32_t _elementBufferHandle;
	ElementStorage _elements;
	IndexBufferUsageType _usageType;

public:
	IndexBuffer() = default;
	IndexBuffer(size_t numElements, IndexBufferUsageType usage);

	// Set the
	void SetData(uint32_t *data);
	void Bind();
};

} // namespace Nimble