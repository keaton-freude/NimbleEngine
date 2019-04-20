#pragma once

/*
 * Basic shader has hard-coded colors, has no transformations, no projection, etc.
 * Its the most basic shader, it just passes the vertices through, colors them some
 * default color.
 *
 */

#include "nimble/opengl-wrapper/IShaderProgram.h"

namespace Nimble {

class BasicShader : public IShaderProgram {
public:
	BasicShader() = default;
	virtual ~BasicShader() override = default;
	
	virtual std::string Name() const {
		return "BasicShader";
	}
};

} // namespace Nimble
