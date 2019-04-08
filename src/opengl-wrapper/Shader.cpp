#include <cassert>
#include <iostream>
#include <GL/glew.h>

#include "nimble/opengl-wrapper/Shader.h"

Shader::Shader() : _loaded(false), _shaderHandle(0) {
}

void Shader::Load() {
	_shaderHandle = glCreateShader(static_cast<GLenum>(this->GetShaderType()));
	_loaded = true;
}

bool Shader::Compile(const char *text) const {
	assert(_loaded && "Load was not called on Shader before compile!");

	glShaderSource(_shaderHandle, 1, &text, nullptr);
	glCompileShader(_shaderHandle);

	int success;
	glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &success);

	return success != 0;
}

void Shader::PrintErrorOutput() const {
	char infoLog[1024];
	glGetShaderInfoLog(_shaderHandle, 1024, nullptr, infoLog);

	std::cout << "[ERR]: Compiling Shader failed. Message: " << infoLog << std::endl;
}
