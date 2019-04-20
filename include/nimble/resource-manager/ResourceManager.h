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

#include "nimble/opengl-wrapper/Shader.h"
#include "nimble/utility/Singleton.h"

namespace Nimble {

class ResourceManager : public Singleton<ResourceManager> {
public:
        // Singleton
        ResourceManager() {
                std::cout << "Resource Root: " << GetResourceRoot() << "\n";
        }

private:

        // Returns a fully-qualified path, based on the passed in name
        const std::string GetPathFromName();

        // Resource Root is set via configuration.. for now it'll be set via a const
        // string which points relative to the build folder
        // In the future, we may make this possible to set (either through some settings)
        // file, or via CLI or something similar.
        const std::string& GetResourceRoot() {
                const static std::string RESOURCE_ROOT = std::filesystem::current_path().string();
                return RESOURCE_ROOT;
        }
public:

        // Give me a shader with @name, which is a path relative to the 'shaders'
        // folder within the resources folder
        std::shared_ptr<Shader> GetShader(const std::string &name); 

        // Other options?
        // Return a mesh by filename
        // Return a material by name
        // etc..
};
}
