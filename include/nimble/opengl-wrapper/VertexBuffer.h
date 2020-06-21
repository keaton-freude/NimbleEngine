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

	VertexBuffer(IMesh *mesh, BufferUsageType usageType) {
		glGenBuffers(1, &_vertexBufferHandle);
		_usageType = usageType;

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
		auto glUsageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, (long)mesh->GetNumBytes(), mesh->GetData(), (GLenum)glUsageType);
	}

	/*
		void SetData(const ElementStorage &data) {
			if(data.size() == _elements.size()) {
				// no need to realloc with operator=
				for(size_t i = 0; i < data.size(); ++i) {
					_elements[i] = data[i];
				}
			} else {
				_elements = data;
			}

			glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
			auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW :
	   GL_DYNAMIC_DRAW; glBufferData(GL_ARRAY_BUFFER, (long)(_elements.size() * T::SizeInBytes()),
	   &_elements[0], (GLenum)usageType);
		}
	*/

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
	}


}; // class VertexBuffer

} // namespace Nimble
