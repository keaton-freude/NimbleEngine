#include "nimble/opengl-wrapper/IndexBuffer.h"
#include <GL/glew.h>
#include <iostream>

using namespace Nimble;

IndexBuffer::IndexBuffer(IMesh *mesh, BufferUsageType usage) : _usageType(usage) {
	_numFaces = mesh->GetNumFaces();
	glGenBuffers(1, &_elementBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
	auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetFacesNumBytes(), mesh->GetFaceData(),
				 static_cast<GLenum>(usageType));
}

/*/void IndexBuffer::SetData(const ElementStorage &data) {
	if(data.size() == _elements.size()) {
		// no need to realloc with operator=
		for(size_t i = 0; i < data.size(); ++i) {
			_elements[i] = data[i];
		}
	} else {
		_elements = data;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
	auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 static_cast<GLenum>(_elements.size() * sizeof(ElementStorage::value_type)),
				 &_elements[0], static_cast<GLenum>(usageType));
}*/

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
}
