#pragma once

// Abstract Base class for Shaders

class Shader {
private:
	bool _loaded;

protected:
	unsigned int _shaderHandle;

	virtual int GetShaderType() = 0; // this must be defined
public:
	Shader();
	virtual ~Shader() = default;

	void Load();
	bool Compile(const char *text) const;
	// Only call this if the compilation failed
	void PrintErrorOutput() const;
	unsigned int GetHandle() const {
		return _shaderHandle;
	}
};