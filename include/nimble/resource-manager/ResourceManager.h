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

#include <memory>
#include <filesystem>
#include <iostream>
#include <cassert>

#include "spdlog/spdlog.h"

#include "nimble/opengl-wrapper/Shader.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/utility/Singleton.h"

namespace Nimble {

class ResourceManager : public Singleton<ResourceManager> {
public:
        // Singleton
        ResourceManager() {
                spdlog::info("Resource Root: {}", GetResourceRoot());
        }

private:

        // Returns a fully-qualified path, based on the passed in name
        const std::string GetPathFromName();

        // Resource Root is set via configuration.. for now it'll be set via a const
        // string which points relative to the build folder
        // In the future, we may make this possible to set (either through some settings)
        // file, or via CLI or something similar.
        const std::string& GetResourceRoot() {
                // We assume the executable is somewhere below: build/frontend/<executable>
                // Our RESOURCE_ROOT is relative toe the build folder, so go 1 folder up 
                const static std::string RESOURCE_ROOT = (std::filesystem::current_path().parent_path() / "resources").string();
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

        // Other options?
        // Return a mesh by filename
        // Return a material by name
        // etc..
};
}
