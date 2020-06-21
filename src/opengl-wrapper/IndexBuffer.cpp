#include "nimble/opengl-wrapper/IndexBuffer.h"
#include <GL/glew.h>

using namespace Nimble;

IndexBuffer::IndexBuffer(IMesh *mesh, BufferUsageType usage) : _usageType(usage) {
	_numFaces = mesh->GetNumFaces();
	glGenBuffers(1, &_elementBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
	auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetFacesNumBytes(), mesh->GetFaceData(),
				 static_cast<GLenum>(usageType));
}

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
}
