#include <GL/glew.h>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <utility>

#include "spdlog/spdlog.h"

#include "nimble/opengl-wrapper/Shader.h"
#include "nimble/opengl-wrapper/FragmentShader.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/VertexShader.h"

using namespace std;

ShaderProgram::ShaderProgram() {
	_programHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	for(const auto &shader : _shaders) {
		glDeleteShader(shader->GetHandle());
	}
}


void ShaderProgram::AddVertexShader(const char *text) {
	unique_ptr<Shader> shader = std::make_unique<VertexShader>();
	shader->Load();
	if(!shader->Compile(text)) {
		shader->PrintErrorOutput();
		throw std::runtime_error("Failed to compile shader");
	}
	_shaders.push_back(std::move(shader));
}

void ShaderProgram::AddFragmentShader(const char *text) {

	unique_ptr<Shader> shader = std::make_unique<FragmentShader>();
	shader->Load();
	if(!shader->Compile(text)) {
		shader->PrintErrorOutput();
		throw std::runtime_error("Failed to compile shader");
	}
	_shaders.push_back(std::move(shader));
}

bool ShaderProgram::LinkShaders() const {
	for(const auto &shader : _shaders) {
		glAttachShader(_programHandle, shader->GetHandle());
	}

	glLinkProgram(_programHandle);

	int success;
	glGetProgramiv(_programHandle, GL_LINK_STATUS, &success);

	if(!success) {
		char buffer[1024];
		glGetProgramInfoLog(_programHandle, 1024, nullptr, buffer);

		spdlog::error("Failed to link shader, message: {}", buffer);
	}

	return success != 0;
}

void ShaderProgram::Use() const {
	glUseProgram(_programHandle);
}
