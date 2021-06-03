#include <GL/glew.h>
#include <memory>
#include <stdexcept>
#include <utility>

#include "spdlog/spdlog.h"

#include "nimble/opengl-wrapper/FragmentShader.h"
#include "nimble/opengl-wrapper/ShaderInfo.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/Utils.h"
#include "nimble/opengl-wrapper/VertexShader.h"

using namespace std;
using namespace Nimble;

ShaderProgram::ShaderProgram(const std::string &name) : _name(name) {
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

void ShaderProgram::Reload(const char *vertexText, const char *fragmentText) {
	glDetachShader(_programHandle, _shaders[0]->GetHandle());
	glDetachShader(_programHandle, _shaders[1]->GetHandle());

	if(!_shaders[0]->Compile(vertexText)) {
		_shaders[0]->PrintErrorOutput();
		throw std::runtime_error("Failed to compile shader");
	}

	if(!_shaders[1]->Compile(fragmentText)) {
		_shaders[1]->PrintErrorOutput();
		throw std::runtime_error("Failed to compile shader");
	}

	if(!LinkShaders()) {
		throw std::runtime_error("Failed to link shaders");
	}
}

bool ShaderProgram::LinkShaders() {
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

	if(!success) {
		return false;
	}

	// Shader is linked, so we can query off the uniforms and attributes
	QueryUniformsAndAttributes();

	return true;
}

void ShaderProgram::Use() const {
	glUseProgram(_programHandle);
}

void ShaderProgram::QueryUniformsAndAttributes() {
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramiv(_programHandle, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
	glGetProgramiv(_programHandle, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

	// Buffer to store attribute/uniform name
	std::vector<GLchar> nameData(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
	// Max bytes OpenGL is allowed to write to the buffer
	const GLsizei bufSize = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
	// Actual amount of bytes OpenGL wrote to the buffer
	GLsizei length;
	// Size of the attribute / uniform in units of "type"
	GLint size;
	// Type of the attribute / uniform
	GLenum type;

	for(int attrib = 0; attrib < numActiveAttribs; ++attrib) {
		glGetActiveAttrib(_programHandle, (GLuint)attrib, bufSize, &length, &size, &type, &nameData[0]);

		if(!length) {
			// Any failure in glGetActiveAttrib results in a zero length
			spdlog::warn("Failed to retrieve Attribute #{} for shader '{}'", attrib, _name);
			continue;
		}

		std::string name((char *)&nameData[0], length);
		Attribute attribute(name, type, GLenumToString(type));
		_shaderInfo.attributes.push_back(attribute);
	}

	for(int uniform = 0; uniform < numActiveUniforms; ++uniform) {
		glGetActiveUniform(_programHandle, (GLuint)uniform, bufSize, &length, &size, &type, &nameData[0]);

		if(!length) {
			// Any failure in glGetActiveUniform results in a zero length
			spdlog::warn("Failed to retrieve Attribute #{} for shader '{}'", uniform, _name);
			continue;
		}

		std::string name((char *)&nameData[0], length);

		// Uniform uniformStruct(name, type, GLenumToString(type), uniform);
		//_shaderInfo.uniforms.emplace(uniformStruct);
		_shaderInfo.uniforms.emplace(std::make_pair(name, Uniform(name, type, GLenumToString(type), uniform)));
	}

	spdlog::info("ShaderInfo for Shader {}: {}", _name, _shaderInfo.ToString());
}

unsigned int ShaderProgram::GetUniformLocation(const string &name) {
	return _shaderInfo.GetUniformPosition(name);
}
