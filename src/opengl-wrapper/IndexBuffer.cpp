#include "nimble/opengl-wrapper/IndexBuffer.h"
#include <GL/glew.h>

using namespace Nimble;

IndexBuffer::IndexBuffer(BufferUsageType usage) : _usageType(usage) {
	glGenBuffers(1, &_elementBufferHandle);
}

void IndexBuffer::LoadFromMesh(IMesh* mesh) {
	_numFaces = mesh->GetNumFaces();
	Bind();
	auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetFacesNumBytes(), mesh->GetFaceData(),
				 static_cast<GLenum>(usageType));
}

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);
}
