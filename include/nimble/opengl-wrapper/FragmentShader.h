#pragma once
#include "Shader.h"

class FragmentShader : public Shader {
public:
	FragmentShader() = default;

protected:
	virtual int GetShaderType() override;
};
