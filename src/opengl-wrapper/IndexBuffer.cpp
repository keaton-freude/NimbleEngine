#include "nimble/opengl-wrapper/IndexBuffer.h"

#include <glad/glad.h>

using namespace Nimble;

IndexBuffer::IndexBuffer(size_t numElements, IndexBufferUsageType usage)
: _elements(numElements * sizeof(ElementStorage::value_type)), _usageType(usage) {
	glGenBuffers(1, &_elementBufferHandle);
}

void IndexBuffer::SetData(ElementStorage::value_type *data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
	auto usageType = _usageType == IndexBufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elements.size(), data, usageType);
}

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
}