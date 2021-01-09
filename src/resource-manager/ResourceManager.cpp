#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/utility/FileUtility.h"
#include <fmt/format.h>

#include "spdlog/spdlog.h"

using namespace Nimble;

std::shared_ptr<ShaderProgram> ResourceManager::GetShader(const std::string &name) {
	// See if it exists in the cache
	for(const auto &[key, value] : _shaderCache) {
		if(key == name) {
			return value;
		}
	}

	// Basic, we need to find the .frag and .vert components, create a shader
	// program, link and check it. If all is good, return the ShaderProgram to user
	ShaderProgram *program = new ShaderProgram(name);

	// We look for shaders relative to the resource root in a shaders folder
	std::filesystem::path shadersDir = std::filesystem::path(GetResourceRoot()) / "shaders";

	std::filesystem::path vertexShaderSource = shadersDir / fmt::format("{}.vert", name.c_str());
	std::filesystem::path fragmentShaderSource = shadersDir / fmt::format("{}.frag", name.c_str());

	spdlog::debug("Vertex: {}", vertexShaderSource.string());
	spdlog::debug("Frag: {}", fragmentShaderSource.string());

	if(!std::filesystem::exists(vertexShaderSource)) {
		throw std::runtime_error(
		fmt::format("Failed to find vertex shader at: {}", vertexShaderSource.string()));
	}

	if(!std::filesystem::exists(fragmentShaderSource)) {
		throw std::runtime_error(
		fmt::format("Failed to find fragment shader at: {}", vertexShaderSource.string()));
	}

	program->AddVertexShader(FileReadAllText(vertexShaderSource.string()).c_str());
	program->AddFragmentShader(FileReadAllText(fragmentShaderSource.string()).c_str());

	if(!program->LinkShaders()) {
		throw std::runtime_error("Failed to link shaders of name " + name);
	}

	std::shared_ptr<ShaderProgram> programToReturn(program);

	_shaderCache[name] = programToReturn;

	return programToReturn;
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string &name) {
	// Check if it exists in the cache, if not throw.
	// All materials are pre-created and registered for lookup
	for(const auto &[key, value] : _materialCache) {
		if(key == name) {
			return value;
		}
	}

	throw std::runtime_error(fmt::format("Failed to find material with name \"{}\"", name).c_str());
}

void ResourceManager::LoadMaterialsFromDisk() {
	// Walk all material files
	std::filesystem::path materialDirectory = std::filesystem::path(GetResourceRoot()) / "materials";
	spdlog::debug("Iterating Material Files...");
	for (auto & path : std::filesystem::directory_iterator(materialDirectory)) {
		if (!path.is_regular_file()) {
			continue;
		}

		spdlog::debug("Loading material at: {}", path.path().string());
		auto material = std::make_shared<Material>();

		try {
			material->LoadFromFile(path.path().string().c_str());
		} catch (const std::exception& ex) {
			spdlog::error("Failed to load material. Error message: {}", ex.what());
		}

		if (material) {
			_materialCache[material->GetName()] = material;			
		} else {
			spdlog::error("Failed to create material at path: {}", path.path().string());
			continue;
		}

	}
}