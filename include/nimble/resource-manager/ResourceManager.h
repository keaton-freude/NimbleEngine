#pragma once

#include <memory>

#include "nimble/opengl-wrapper/Shader.h"

namespace Nimble {

class ResourceManager {
private:
        // Singleton
        ResourceManager() = default;
public:
        const ResourceManager& Get() {
                static ResourceManager resourceManager;
                return resourceManager;
        }

        // Give me a shader
        std::shared_ptr<Shader> GetShader(const std::string name); 

};

}
