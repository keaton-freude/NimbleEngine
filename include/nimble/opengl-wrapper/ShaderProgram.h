#pragma once

#include "Shader.h"
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nimble/core/Assert.h>

#include "nimble/opengl-wrapper/ShaderInfo.h"

// Shader program is some combination of shaders
// which can be bound to the pipeline. Typically this is Vert & Frag
// but could include compute shaders, geometry shaders, etc.
namespace Nimble {
class ShaderProgram {
private:
	std::vector<std::unique_ptr<Shader>> _shaders;
	unsigned int _programHandle;
	std::string _name;
	ShaderInfo _shaderInfo;

public:
	explicit ShaderProgram(const std::string &name);
	~ShaderProgram();

	void AddVertexShader(const char *text);
	void AddFragmentShader(const char *text);
	void Reload(const char *vertexText, const char *fragmentText);

	// Link all of the attached shaders
	bool LinkShaders();

	// Bind the shader program to the pipeline
	void Use() const;

	[[nodiscard]] unsigned int ShaderHandle() const {
		return _programHandle;
	}

	unsigned int GetUniformLocation(const std::string& name);

	template <typename T>
	void SetUniform(const std::string &name, const T &t) {
		DIE("SetUniform called with a type without a specialization. Type: {}", typeid(T).name());
	}

	template <typename T>
	void SetUniform(int, const T &t) {
		DIE("SetUniform called with a type without a specialization. Type: {}", typeid(T).name());
	}


private:
	// Support for querying the uniforms and attributes for the linked shaders
	void QueryUniformsAndAttributes();
};

// Partial specializations
template <>
inline void ShaderProgram::SetUniform<glm::vec3>(const std::string &name, const glm::vec3 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform3fv(location, 1, &t[0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::vec2>(const std::string &name, const glm::vec2 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform2fv(location, 1, &t[0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::vec4>(const std::string &name, const glm::vec4 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform4fv(location, 1, &t[0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat4>(const std::string &name, const glm::mat4 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(t));
}

template <>
inline void ShaderProgram::SetUniform<glm::mat4x2>(const std::string &name, const glm::mat4x2 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix4x2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat4x3>(const std::string &name, const glm::mat4x3 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix4x3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat3>(const std::string &name, const glm::mat3 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat3x2>(const std::string &name, const glm::mat3x2 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix3x2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat3x4>(const std::string &name, const glm::mat3x4 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix3x4fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat2>(const std::string &name, const glm::mat2 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat2x3>(const std::string &name, const glm::mat2x3 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix2x3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<glm::mat2x4>(const std::string &name, const glm::mat2x4 &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniformMatrix2x4fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
inline void ShaderProgram::SetUniform<bool>(const std::string &name, const bool &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform1i(location, t);
}

template <>
inline void ShaderProgram::SetUniform<int>(const std::string &name, const int &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform1i(location, t);
}

template <>
inline void ShaderProgram::SetUniform<float>(const std::string &name, const float &t) {
	auto location = _shaderInfo.GetUniformPosition(name);
	glUniform1f(location, t);
}

// -- Set Uniform via already known location

template <>
inline void ShaderProgram::SetUniform<glm::vec3>(int location, const glm::vec3 &t) {
	glUniform3fv(location, 1, &t[0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::vec2>(int location, const glm::vec2 &t) {
	glUniform2fv(location, 1, &t[0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::vec4>(int location, const glm::vec4 &t) {
	glUniform4fv(location, 1, &t[0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat4>(int location, const glm::mat4 &t) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(t));
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat4x2>(int location, const glm::mat4x2 &t) {
	glUniformMatrix4x2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat4x3>(int location, const glm::mat4x3 &t) {
	glUniformMatrix4x3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat3>(int location, const glm::mat3 &t) {
	glUniformMatrix3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat3x2>(int location, const glm::mat3x2 &t) {
	glUniformMatrix3x2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat3x4>(int location, const glm::mat3x4 &t) {
	glUniformMatrix3x4fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat2>(int location, const glm::mat2 &t) {
	glUniformMatrix2fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat2x3>(int location, const glm::mat2x3 &t) {
	glUniformMatrix2x3fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<glm::mat2x4>(int location, const glm::mat2x4 &t) {
	glUniformMatrix2x4fv(location, 1, GL_FALSE, &t[0][0]);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<bool>(int location, const bool &t) {
	glUniform1i(location, t);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<int>(int location, const int &t) {
	glUniform1i(location, t);
}

template <>
[[maybe_unused]] inline void ShaderProgram::SetUniform<float>(int location, const float &t) {
	glUniform1f(location, t);
}

} // namespace Nimble