#include "nimble/material/Material.h"

#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/utility/FileUtility.h"
#include "simdjson.h"
#include <utility>

using namespace std;
using namespace Nimble;
using namespace simdjson;

Material::Material(const string &name, const string &shaderName)
: _name(name), _shader(ResourceManager::Get().GetShader(shaderName)) {
}

Material::Material(const string &name, shared_ptr<ShaderProgram> shader)
: _name(name), _shader(std::move(shader)) {
}

std::shared_ptr<Material> Material::CreateMaterial(const std::string& name, const std::string& shaderName) {
	return std::make_shared<Material>(name, shaderName);
}

void Material::LoadFromFile(const char* path) {
	// Parse for all settings, if there are no parse errors this should be considered valid
	dom::parser parser;
	dom::element root = parser.load(path);

	_name = root["name"];
	_shader_name = root["shader"];

	dom::array textures;
	auto error = root["textures"].get(textures);

	if (!error) {
		for (auto texture : textures) {
			auto setting = std::make_unique<TextureMaterialSetting>();
			setting->Load(texture);
			_settings.push_back(std::move(setting));
		}
	}

	_shader = ResourceManager::Get().GetShader(_shader_name);

	dom::element receivesLighting;
	error = root["receives_lighting"].get(receivesLighting);

	if (!error && receivesLighting.is_bool()) {
		_receivesLighting = receivesLighting.get_bool();
	} else {
		_receivesLighting = false;
	}
}

void Material::Bind() {
	_shader->Use();

	for(const auto& setting : _settings) {
		setting->Bind();
		// Todo, associate sampler with texture setting
		_sampler.Bind();
	}
}

void TextureMaterialSetting::Load(const simdjson::dom::element &element) {
	_texture = std::make_shared<TextureUnit>();
	_texture->slot = element["slot"];

	if (element.at_key("resource_name").is_object() && element.at_key("resource_name").at_key("user_specified").get_bool()) {
		// This is a user-specified value. Creator of material is required to specify the value at some point
		// before binding the material
		_resolved = false;
	} else if (element.at_key("resource_name").is_string()) {
		// If it's not a user-specified dict, we require the texture path to be present
		SetTexturePath(element.at_key("resource_name").get_string().first.data());
	} else {
		assert(false && "Material texture path is neither user specified, or a string!");
	}
}

void TextureMaterialSetting::SetTexturePath(const std::string &path) {
	_texture->texture = ResourceManager::Get().GetTexture2D(path);
	_resolved = true;
}

void TextureMaterialSetting::Bind() {
	assert(_resolved && "Material settings not resolved!");
	glActiveTexture(GL_TEXTURE0 + (GLenum)_texture->slot);
	glBindTexture(GL_TEXTURE_2D, _texture->texture->GetTextureHandle());
}