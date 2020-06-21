#include "nimble/material/Material.h"

#include "nimble/resource-manager/ResourceManager.h"
#include <utility>

using namespace std;
using namespace Nimble;

Material::Material(const string &name, const string &shaderName)
: _name(name), _shader(ResourceManager::Get().GetShader(shaderName)) {
	ResourceManager::Get().AddMaterial(name, this);
}

Material::Material(const string &name, shared_ptr<ShaderProgram> shader)
: _name(name), _shader(std::move(shader)) {
	ResourceManager::Get().AddMaterial(name, this);
}

void Material::Bind() {
	_shader->Use();
}