#pragma once

#include "Shader.h"
#include <list>
#include <memory>

// Shader program is some combination of shaders
// which can be bound to the pipeline. Typically this is Vert & Frag
// but could include compute shaders, geometry shaders, etc.

class ShaderProgram {
private:
	std::list<std::unique_ptr<Shader>> _shaders;
	unsigned int _programHandle;

public:
	ShaderProgram();
	~ShaderProgram();

	void AddVertexShader(const char *text);
	void AddFragmentShader(const char *text);

	// Link all of the attached sahders
	bool LinkShaders() const;

	// Bind the shader program to the pipeline
	void Use() const;

	const unsigned int ShaderHandle() const {
		return _programHandle;
	}
};