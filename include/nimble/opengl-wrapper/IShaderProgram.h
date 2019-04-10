#pragma once

#include <string>

namespace Nimble {

class IShaderProgram {
public:
	IShaderProgram() = default;
	virtual ~IShaderProgram() = default;

	virtual std::string Name() = 0;
	virtual IShaderProgram *Create() = 0;


	bool LinkShaders() const;
	void Use() const;
};

} // namespace Nimble
