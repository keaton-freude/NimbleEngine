#pragma once


namespace Nimble {
class VertexArrayObject {
private:
	uint32_t _vaoHandle;

public:
	VertexArrayObject() {
		glGenVertexArrays(1, &_vaoHandle);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, _vaoHandle);
	}

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
}