#include "nimble/material/Material.h"

#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/utility/FileUtility.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"
#include <utility>
#include <memory>

using namespace std;
using namespace Nimble;
using namespace rapidjson;

Material::Material(string name)
: _name(std::move(name)) {
}

std::shared_ptr<Material> Material::CreateMaterial(const std::string& name) {
	return std::make_shared<Material>(name);
}

static TextureUnit GetTextureUnitFromJson(const Value& texture_obj) {
	ASSERT(texture_obj.HasMember("texture_uri"), "texture_uri must be a key in diffuse_texture");
	ASSERT(texture_obj["texture_uri"].IsString(), "texture_uri must be a string!");

	// Retrieve the texture, or load it for the first time if this is our first time seeing it
	auto texture_uri = texture_obj["texture_uri"].GetString();
	std::shared_ptr<Texture2D> texture = ResourceManager::Get().GetTexture2D(texture_uri);

	// Load sampler settings
	SamplerSettings settings;

	if (texture_obj.HasMember("sampler_settings")) {
		// If user has specified settings, extract them here
		ASSERT(texture_obj["sampler_settings"].IsObject(), "Sampler settings must be an object. Check the schema.");
		auto sampler_settings = texture_obj["sampler_settings"].GetObject();

		if (sampler_settings.HasMember("texture_wrap_mode_u")) {
			ASSERT(sampler_settings["texture_wrap_mode_u"].IsString(), "texture_wrap_mode_u must be a string.");
			settings.textureWrapModeU = SamplerSettingsUtil::TextureWrapModeFromString(
			sampler_settings["texture_wrap_mode_u"].GetString());
		}

		if (sampler_settings.HasMember("texture_wrap_mode_v")) {
			ASSERT(sampler_settings["texture_wrap_mode_v"].IsString(), "texture_wrap_mode_v must be a string.");
			settings.textureWrapModeV = SamplerSettingsUtil::TextureWrapModeFromString(
			sampler_settings["texture_wrap_mode_v"].GetString());
		}

		if (sampler_settings.HasMember("texture_mag_filter")) {
			ASSERT(sampler_settings["texture_mag_filter"].IsString(), "texture_mag_filter must be a string.");
			settings.textureMagFilter = SamplerSettingsUtil::TextureMagFilterFromString(
			sampler_settings["texture_mag_filter"].GetString());
		}

		if (sampler_settings.HasMember("texture_min_filter")) {
			ASSERT(sampler_settings["texture_min_filter"].IsString(), "texture_min_filter must be a string.");
			settings.textureMinFilter = SamplerSettingsUtil::TextureMinFilterFromString(
			sampler_settings["texture_min_filter"].GetString());
		}
	}

	// Even if user doesn't specify sampler settings, we'll just take the defaults we started with
	return TextureUnit(texture, Sampler());
}

void Material::LoadFromFile(const char* path) {
	Document document;
	document.Parse(FileReadAllText(path).c_str());

	ASSERT(!document.HasParseError(), "JSON parsing for {} failed.\n\nDetails: {}", path, GetParseError_En(document.GetParseError()));

	ASSERT(document.HasMember("name") && document["name"].IsString(), "Material JSON is missing `name` field, or it is not a string.");
	_name = document["name"].GetString();

	ASSERT(document.HasMember("material_type") && document["material_type"].IsString(), "Material JSON is missing `material_type` field, or it is not a string.");

	if (document.HasMember("diffuse_texture")) {
		// If a texture key is present, validate required keys are present
		ASSERT(document["diffuse_texture"].IsObject(), "Diffuse Texture element is not an object. Check the schema.");
		_diffuse_texture = GetTextureUnitFromJson(document["diffuse_texture"].GetObject());
	}

	if (document.HasMember("normal_texture")) {
		ASSERT(document["normal_texture"].IsObject(), "Normal Texture element is not an object. Check the schema.");
		_normal_texture = GetTextureUnitFromJson(document["normal_texture"].GetObject());
	}

	if (document.HasMember("receives_lighting")) {
		ASSERT(document["receives_lighting"].IsBool(), "receives_lighting key must be a boolean");
		_receives_lighting = document["receives_lighting"].GetBool();
	} else {
		_receives_lighting = true;
	}
}

const std::string& Material::GetName() const {
	return _name;
}

std::optional<bool> Material::GetReceivesLighting() const {
	return _receives_lighting;
}

std::optional<TextureUnit> Material::GetDiffuseTexture() const {
	return _diffuse_texture;
}

std::optional<TextureUnit> Material::GetNormalTexture() const {
	return _normal_texture;
}

MaterialType Material::GetMaterialType() const {
	return _material_type;
}