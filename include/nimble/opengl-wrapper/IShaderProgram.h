#pragma once

#include <string>

namespace Nimble {

class IShaderProgram {
public:
	IShaderProgram() = default;
	virtual ~IShaderProgram() = default;

	virtual std::string Name() const = 0;

	bool LinkShaders() const;
	void Use() const;
};

} // namespace Nimble
