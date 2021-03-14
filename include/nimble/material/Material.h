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

#include "simdjson.h"


namespace Nimble {

// TODO: Name? Combines a texture pointer along with
// the name of the uniform it should be bound to
struct TextureUnit {
	std::shared_ptr<Texture2D> texture;
	size_t slot;
};

enum class MaterialSettingType {
	BOOLEAN,
	STRING,
	// A setting type of Invalid aborts the program with an error
	INVALID
};

// Represents a generic setting within a material file
// and is typically used with user-settable values, such as
// enable or disabling lighting, shadows, texture units, etc.
class IMaterialSetting {
protected:
	// The name/key of the setting
	std::string _name;

	// may be useful for introspection
	MaterialSettingType _type = MaterialSettingType::INVALID;

	bool _required = false;
	bool _resolved = true;

public:
	~IMaterialSetting() = default;

	// Load the setting from its JSON representation
	virtual void Load(const simdjson::dom::element& element) = 0;
	bool Resolved() const {
		return _resolved;
	}
	virtual void Bind() = 0;
};

class BooleanMaterialSetting : public IMaterialSetting {
private:
	bool _value;

public:
	virtual ~BooleanMaterialSetting() = default;

	void Load(const simdjson::dom::element& element) override {

	}

	void Bind() override {
		// Nothing to do
	}
};

class TextureMaterialSetting : public IMaterialSetting {
private:
	std::shared_ptr<TextureUnit> _texture;

public:
	void Load(const simdjson::dom::element& element) override;
	void SetTexturePath(const std::string& path);
	void Bind() override;
};

class Material {
private:
	std::string _name;
	std::string _shader_name;

	std::shared_ptr<ShaderProgram> _shader = nullptr;

	Sampler _sampler{};
	bool _valid = false;

	std::list<std::unique_ptr<IMaterialSetting>> _settings;

	bool _receivesLighting = true;

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

	bool GetReceivesLighting() const {
		return _receivesLighting;
	}

	bool Resolved() {
		for (const auto& setting : _settings) {
			if (!setting->Resolved()) {
				return false;
			}
		}

		return true;
	}

	std::shared_ptr<Material> Clone() {
		auto clone = std::make_shared<Material>();
		assert(false);
		return clone;
	}

private:
};
} // namespace Nimble