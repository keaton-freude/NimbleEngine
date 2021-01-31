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
			spdlog::info("Texture name: {}", texture["name"].get_string().value());

			TextureUnit tu{};

			tu.uniform_name = texture["uniform_name"];
			tu.texture = ResourceManager::Get().GetTexture2D((std::string)texture["resource_name"]);

			_textures.push_back(tu);
		}
	}

	_shader = ResourceManager::Get().GetShader(_shader_name);

	// Associate textures
	int i = 0;
	for(const auto& texture : _textures) {
		_shader->SetUniform(texture.uniform_name, i++);
	}

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

	int i = 0;
	for (const auto& texture : _textures) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture.texture->GetTextureHandle());
		_sampler.Bind();
		i++;
	}
}