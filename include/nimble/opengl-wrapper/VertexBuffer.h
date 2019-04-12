#pragma once

#include <GL/glew.h>
#include <bits/stdint-uintn.h>
#include <stddef.h>
#include <vector>

#include "nimble/opengl-wrapper/Buffer.h"

namespace Nimble {

template <typename T>
class VertexBuffer {
public:
	typedef std::vector<T> ElementStorage;

private:
	std::vector<T> _elements;
	uint32_t _vertexBufferHandle;
	BufferUsageType _usageType;

public:
	// TODO: Consider deleting defaults? Are they useful?
	VertexBuffer() = default;

	// Just creates buffer and allocates space in the vector
	VertexBuffer(size_t numElements, BufferUsageType usage)
	: _elements(numElements * sizeof(ElementStorage::value_type)), _usageType(usage) {
		glGenBuffers(1, &_vertexBufferHandle);
	}

	// const ref data, upload immediately, do not take ownership of the vector
	VertexBuffer(const ElementStorage &elements, BufferUsageType usage)
	: _elements(elements), _usageType(usage) {
		glGenBuffers(1, &_vertexBufferHandle);
		SetData(_elements);
	}

	// Take ownership of the elements
	VertexBuffer(ElementStorage &&elements, BufferUsageType usage)
	: _elements(elements), _usageType(usage) {
		glGenBuffers(1, &_vertexBufferHandle);
		SetData(_elements);
	}

	// NOTE: Do not support copy semantics of _elements (maybe if needed?)

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
		auto usageType = _usageType == BufferUsageType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, (long)(_elements.size() * T::SizeInBytes), &_elements[0], (GLenum)usageType);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
	}


}; // class VertexBuffer

} // namespace Nimble
