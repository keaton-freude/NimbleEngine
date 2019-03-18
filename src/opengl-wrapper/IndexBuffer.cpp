#include "nimble/opengl-wrapper/IndexBuffer.h"

#include <glad/glad.h>

using namespace Nimble;

IndexBuffer::IndexBuffer(size_t numElements, BufferUsageType usage)
: _elements(numElements), _usageType(usage) {
	glGenBuffers(1, &_elementBufferHandle);
}

void IndexBuffer::SetData(const ElementStorage &data) {
	if(data.size() == _elements.size()) {
		// no need to realloc with operator=
		for(int i = 0; i < data.size(); ++i) {
			_elements[i] = data[i];
		}
	} else {
		_elements = data;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
	auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elements.size() * sizeof(ElementStorage::value_type),
				 &_elements[0], usageType);
}

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
}