#pragma once

#include "Shader.h"

class VertexShader : public Shader {
public:
	VertexShader() = default;

protected:
	virtual int GetShaderType() override;
};