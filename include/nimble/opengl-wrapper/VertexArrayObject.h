#pragma once

#include "GL/glew.h"
#include <functional>

namespace Nimble {
class VertexArrayObject {
private:
	uint32_t _vaoHandle;
	std::function<void()> _setVertexAttribs;

public:
	VertexArrayObject(std::function<void()> setVertexAttribsFunc)
	: _setVertexAttribs(setVertexAttribsFunc) {
		glGenVertexArrays(1, &_vaoHandle);
	}

	void Bind() {
		glBindVertexArray(_vaoHandle);
	}

	void Unbind() {
		glBindVertexArray(0);
	}

	void SetVertexAttribs() {
		_setVertexAttribs();
	}
};
}