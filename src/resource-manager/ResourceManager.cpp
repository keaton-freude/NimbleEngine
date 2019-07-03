#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/BasicShader.h"
#include <fmt/format.h>
#include "nimble/utility/FileUtility.h"

#include "spdlog/spdlog.h"

using namespace Nimble;

std::shared_ptr<ShaderProgram> ResourceManager::GetShader(const std::string &name) {
    // User provides some name like
    // Basic, we need to find the .frag and .vert components, create a shader
    // program, link and check it. If all is good, return the ShaderProgram to user
    ShaderProgram *program = new ShaderProgram();

    // We look for shaders relative to the resource root in a shaders folder
    std::filesystem::path shadersDir = std::filesystem::path(GetResourceRoot()) / "shaders";

    std::filesystem::path vertexShaderSource = shadersDir / fmt::format("{}.vert", name.c_str());
    std::filesystem::path fragmentShaderSource = shadersDir / fmt::format("{}.frag", name.c_str());

    spdlog::debug("Vertex: {}", vertexShaderSource.string());
    spdlog::debug("Frag: {}", fragmentShaderSource.string());

    if (!std::filesystem::exists(vertexShaderSource)) {
        throw std::runtime_error(fmt::format("Failed to find vertex shader at: {}", vertexShaderSource.string()));
    }

    if (!std::filesystem::exists(fragmentShaderSource)) {
        throw std::runtime_error(fmt::format("Failed to find fragment shader at: {}", vertexShaderSource.string()));
    }

    program->AddVertexShader(FileReadAllText(vertexShaderSource).c_str());
    program->AddFragmentShader(FileReadAllText(fragmentShaderSource).c_str());

    if (!program->LinkShaders()) {
        throw std::runtime_error("Failed to link shaders of name " + name);
    }

    return std::shared_ptr<ShaderProgram>(program);
}