#pragma once

/*
 * A material is a collection of settings which determine a number of settings related
 * to rendering a particular mesh, able to achieve a specific desired look.
 *
 * A Material includes:
 * - 0 or more Textures
 * - A bound ShaderProgram
 * ---- 0 or more exposed Shader settings, which need to be set
 * ---- prior to rendering
 * - Material-specific settings
 *
 * A Material can be `Bound` which means its settings are applied to the current OpenGL state
 * but no draw calls have occured, just state.
 *
 * In the future, Materials will be defined via some intermediate format, such as JSON
 * For now.. Materials will be C++ objects, created and edited via mutation functions
 *
 * Materials, when they are created, are automatically added to the ResourceManager and may
 * be looked up by name later.
 *
 *

*/

#include <memory>
#include <string>

#include "nimble/opengl-wrapper/ShaderProgram.h"


namespace Nimble {
class Material {
private:
	// All materials have a name, setup upon construction which cannot change
	const std::string _name;

	// All materials must contain a ShaderProgram, set upon construction which cannot change
	const std::shared_ptr<ShaderProgram> _shader;

	// TODO: Textures here

public:
	Material() = default;
	// Create material with name, do a lookup for shader
	Material(std::string name, std::string shaderName);
	// Create material with name, provide a shared_ptr to specified ShaderProgram
	Material(std::string name, std::shared_ptr<ShaderProgram> shader);

	// Bind this materials state to the pipeline
	void Bind();

	std::shared_ptr<ShaderProgram> GetShader() {
		return _shader;
	}

private:
};
} // namespace Nimble