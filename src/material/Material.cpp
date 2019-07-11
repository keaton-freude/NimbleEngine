#include "nimble/material/Material.h"
#include "nimble/resource-manager/ResourceManager.h"

using namespace std;
using namespace Nimble;

Material::Material(string name, string shaderName)
: _name(name), _shader(ResourceManager::Get().GetShader(shaderName)) {
	ResourceManager::Get().AddMaterial(name, this);
}

Material::Material(string name, shared_ptr<ShaderProgram> shader) : _name(name), _shader(shader) {
	ResourceManager::Get().AddMaterial(name, this);
}

void Material::Bind() {
	_shader->Use();
}

bool Material::IsRegistered() {
}