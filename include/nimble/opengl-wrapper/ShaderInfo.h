/*
	ShaderInfo is a structure which contains stats and information for a shader
	Useful for debugability, and building abstractions over shaders without parsing
	the glsl code itself
*/

#pragma once

#include "GL/glew.h"
#include <exception>
#include <fmt/format.h>

#include <sstream>
#include <string>
#include <vector>

namespace Nimble {

// A uniform is a piece of data, accessible to the shader and set by the CPU
// The uniform data is _constant_ across all vertices
struct Uniform {
	std::string name;
	// Shape of the data, like vec2f, mat4x4, etc
	GLenum type;
	std::string typeName;
	GLuint location;

	Uniform(const std::string &name, GLenum type, const std::string &typeName, GLuint location)
	: name(name), type(type), typeName(typeName), location(location) {
	}

	std::string ToString() const {
		return fmt::format("Uniform Name: {} Type: {}", name, typeName);
	}
};

// An attribute is a piece of data, accessible to the shader and set by the CPU
// The attribute data is _varying_ across all vertices
struct Attribute {
	std::string name;
	// Shape of the data, like vec2f, mat4x4, etc
	GLenum type;
	std::string typeName;

	Attribute(const std::string &name, GLenum type, const std::string &typeName)
	: name(name), type(type), typeName(typeName) {
	}

	std::string ToString() const {
		return fmt::format("Attribute Name: {} Type: {}", name, typeName);
	}
};

struct ShaderInfo {
	std::vector<Attribute> attributes;
	std::vector<Uniform> uniforms;

	std::string ToString() {
		// Dump all info from attributes & uniforms
		std::stringstream ss;

		ss << "Attributes:\n";
		for(const auto &attribute : attributes) {
			ss << attribute.ToString() << "\n";
		}

		ss << "\nUniforms:\n";
		for(const auto &uniform : uniforms) {
			ss << uniform.ToString() << "\n";
		}

		return ss.str();
	}

	GLuint GetUniformPosition(const std::string &name) {
		for(const auto &uniform : uniforms) {
			if(uniform.name == name) {
				return uniform.location;
			}
		}

		throw std::runtime_error(fmt::format("Failed to find uniform with name {}", name));
	}
};
} // namespace Nimble