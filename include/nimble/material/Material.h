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
#include <vector>

#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/Texture2D.h"
#include "nimble/opengl-wrapper/Sampler.h"


namespace Nimble {

// TODO: Name? Combines a texture pointer along with
// the name of the uniform it should be bound to
struct TextureUnit {
	std::shared_ptr<Texture2D> texture;
	std::string uniform_name;
};

class Material {
private:
	std::string _name;
	std::string _shader_name;

	std::shared_ptr<ShaderProgram> _shader = nullptr;

	// 0 or more textures which should be bound when the material is bound
	std::vector<TextureUnit> _textures{};
	Sampler _sampler{};

	bool _valid = false;

	bool _receivesLighting = false;

public:
	Material() = default;
	// Create material with name, do a lookup for shader
	Material(const std::string &name, const std::string &shaderName);
	// Create material with name, provide a shared_ptr to specified ShaderProgram
	Material(const std::string &name, std::shared_ptr<ShaderProgram> shader);

	// Factory Methods
	static std::shared_ptr<Material> CreateMaterial(const std::string& name, const std::string& shaderName);

	void LoadFromFile(const char* path);

	// Bind this materials state to the pipeline
	void Bind();
	const std::string& GetName() const {
		return _name;
	}

	std::shared_ptr<ShaderProgram> GetShader() {
		return _shader;
	}

	bool GetReceivesLighting() {
		return _receivesLighting;
	}

private:
};
} // namespace Nimble