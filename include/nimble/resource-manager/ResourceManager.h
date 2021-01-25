#pragma once

/*
 * Users provide resource lookups via relative paths, such as:
 * 'GetShader at basic/basic'
 * The resource manager knows we need to construct a new shader
 * such that we create and load a ShaderProgram pointing to both
 * '<somePath>/basic/basic.vert, <somePath>/basic/basic.frag
 * Which is a "Configuration by Convention" decision taken here
 *
 * Other types of "Get<X>" resources do similar things, refer to docs
 * for them
 */

#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "nimble/IMesh.h"
#include "nimble/Mesh.h"
#include "nimble/material/Material.h"
#include "nimble/opengl-wrapper/Shader.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/utility/Singleton.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <nimble/utility/FileUtility.h>

namespace Nimble {

class ResourceManager : public Singleton<ResourceManager> {
private:
	// Keep a cache of each of our resource types
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaderCache;
	std::unordered_map<std::string, std::shared_ptr<Material>> _materialCache;
	std::unordered_map<std::string, std::shared_ptr<IMesh>> _meshCache;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> _texture2DCache;

public:
	// Singleton
	ResourceManager() {
		spdlog::info("Resource Root: {}", GetResourceRoot());
	}

private:
	// Returns a fully-qualified path, based on the passed in name
	std::string GetPathFromName(const std::string &resourceType, const std::string &name) {
		// resourceType is the name of a folder in the resource folder
		spdlog::debug("GetPathFromName resourceType: {}, name: {}", resourceType, name);
		std::filesystem::path subDir = std::filesystem::path(GetResourceRoot()) / resourceType;
		spdlog::debug("Resource subDir: {}", subDir.string());
		std::filesystem::path fullPath = subDir / name;
		spdlog::debug("Full path: {}", fullPath.string());

		// return std::string(fullPath.c_str());
		return fullPath.string();
	}

	// Resource Root is set via configuration.. for now it'll be set via a const
	// string which points relative to the build folder
	// In the future, we may make this possible to set (either through some settings)
	// file, or via CLI or something similar.
	static const std::string &GetResourceRoot() {
		// We assume the resource directory is next to the executable
		const static std::string RESOURCE_ROOT = (std::filesystem::current_path() / "resources").string();
		assert(RESOURCE_ROOT.length() != 0 && "RESOURCE_ROOT is an empty string!");

		// NOTE: We are making a hard assumption about the path of the executable
		// but all components which need paths ought use this (extract to class if that
		// usage ever leaves the ResourceManager), it'll be easy to change

		return RESOURCE_ROOT;
	}

public:
	// Give me a shader with @name, which is a path relative to the 'shaders'
	// folder within the resources folder
	std::shared_ptr<ShaderProgram> GetShader(const std::string &name);
	std::shared_ptr<Material> GetMaterial(const std::string &name);

	// The `materials` folder in the resource root contains material definition files, which define
	// the structure and settings for a material. This decouples the material definition from the
	// compiled code, allowing hot reload and easy testing & manipulation of materials
	void LoadMaterialsFromDisk();

	// Mesh
	std::shared_ptr<IMesh> GetMesh(const std::string &name) {
		Assimp::Importer importer;
		auto path = GetPathFromName("models", name);

		auto cachedMesh = _meshCache.find(path);
		if(cachedMesh != _meshCache.end()) {
			return cachedMesh->second;
		}

		const auto _scene =
		importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
		if(!_scene) {
			const auto error = importer.GetErrorString();
			spdlog::error("Failed to load model from path: {}\nError: {}", path, error);
			return nullptr;
		}

		// For now.. just make sure we have a Mesh and just load the first one
		if(!_scene->HasMeshes()) {
			spdlog::error("No meshes found for the model: {}", path);
			return nullptr;
		}
		auto mesh = MeshFactory::FromFile(_scene->mMeshes[0]);
		_meshCache[path] = mesh;
		return mesh;
	}

	std::shared_ptr<Texture2D> GetTexture2D(const std::string& name) {
		auto path = GetPathFromName("textures", name);
		auto cachedTexture = _texture2DCache.find(path);
		if (cachedTexture != _texture2DCache.end()) {
			return cachedTexture->second;
		}

		auto texture = std::make_shared<Texture2D>();
		texture->LoadFromFile(path);

		_texture2DCache[path] = texture;
		return texture;
	}

	void ReloadShaders() {
		for(const auto& [k, v] : _shaderCache) {
			// We look for shaders relative to the resource root in a shaders folder
			std::filesystem::path shadersDir = std::filesystem::path(GetResourceRoot()) / "shaders";

			std::filesystem::path vertexShaderSource = shadersDir / fmt::format("{}.vert", k.c_str());
			std::filesystem::path fragmentShaderSource = shadersDir / fmt::format("{}.frag", k.c_str());

			v->Reload(FileReadAllText(vertexShaderSource.string()).c_str(), FileReadAllText(fragmentShaderSource.string()).c_str());
		}
	}
};
} // namespace Nimble
