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

#include <list>
#include <memory>
#include <nimble/utility/Singleton.h>
#include <string>
#include <vector>

#include "nimble/opengl-wrapper/Sampler.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/opengl-wrapper/Texture2D.h"

#include <optional>

#include "simdjson.h"


namespace Nimble {

struct TextureUnit {
	TextureUnit() = default;
	TextureUnit(std::shared_ptr<Texture2D> texture, Sampler sampler) :
 		texture(texture), sampler(sampler) {}

	std::shared_ptr<Texture2D> texture;
	Sampler sampler;
};

class Material {
private:
	std::string _name;
	std::string _shader_name;

	// Compiled shader program, built from N shaders
	std::shared_ptr<ShaderProgram> _shader = nullptr;

	std::optional<TextureUnit> _diffuse_texture{};
	std::optional<TextureUnit> _normal_texture{};

	// Whether the object receives lighting or not
	std::optional<bool> _receives_lighting{};

public:
	Material() = default;
	// Create material with name, do a lookup for shader
	Material(std::string name, const std::string &shaderName);
	// Create material with name, provide a shared_ptr to specified ShaderProgram
	Material(std::string name, std::shared_ptr<ShaderProgram> shader);

	// Factory Methods
	static std::shared_ptr<Material> CreateMaterial(const std::string& name, const std::string& shaderName);

	void LoadFromFile(const char* path);

	[[nodiscard]] const std::string& GetName() const;
	[[nodiscard]] std::shared_ptr<ShaderProgram> GetShader() const;

	[[nodiscard]] std::optional<bool> GetReceivesLighting() const;
	[[nodiscard]] std::optional<TextureUnit> GetDiffuseTexture() const;
	[[nodiscard]] std::optional<TextureUnit> GetNormalTexture() const;

	// TODO throw this away
	void Bind() {
		_shader->Use();

		if (_diffuse_texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
			_diffuse_texture.value().texture->GetTextureHandle());
		}
	}

private:
};
} // namespace Nimble