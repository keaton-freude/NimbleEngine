#pragma once

#include "Shader.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "nimble/opengl-wrapper/ShaderInfo.h"

// Shader program is some combination of shaders
// which can be bound to the pipeline. Typically this is Vert & Frag
// but could include compute shaders, geometry shaders, etc.
namespace Nimble {
class ShaderProgram {
private:
	std::list<std::unique_ptr<Shader>> _shaders;
	unsigned int _programHandle;
	std::string _name;
	ShaderInfo _shaderInfo;

public:
	ShaderProgram(const std::string &name);
	~ShaderProgram();

	void AddVertexShader(const char *text);
	void AddFragmentShader(const char *text);

	// Link all of the attached sahders
	bool LinkShaders();

	// Bind the shader program to the pipeline
	void Use() const;

	const unsigned int ShaderHandle() const {
		return _programHandle;
	}

private:
	// Support for querying the uniforms and attributes for the linked shaders
	void QueryUniformsAndAttributes();
};
} // namespace Nimble