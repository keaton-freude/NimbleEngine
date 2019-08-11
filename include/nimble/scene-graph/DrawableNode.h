#pragma once

/**
 * Represents a node in the scene graph which results in drawing
 * some geometry to the screen. User decides what mesh & material
 * to use. World Matrix is retrieved from the passed in SceneState
 * transform
 */

#include "nimble/Mesh.h"
#include "nimble/material/Material.h"
#include "nimble/opengl-wrapper/IndexBuffer.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/scene-graph/SceneNode.h"

#include <memory>


namespace Nimble {

// T is the VertexAttribute type
template <typename T>
class DrawableNode : public SceneNode {
private:
	// Require at least a vertex buffer and index buffer
	VertexBuffer _vb;
	IndexBuffer _ib;
	unsigned int _vao;

	// Material slot, required
	std::shared_ptr<Material> _material;

public:
	// Drawable node from pre-existing resources
	DrawableNode(const IMesh *mesh, std::shared_ptr<Material> material)
	: _vb(mesh), _ib(mesh), _material(material) {
	}

	// Drawabl enode with resource names
	DrawableNode(const std::string &meshName, const std::string &materialName) {
		auto mesh = ResourceManager::Get().GetMesh(meshName);
		_vb = VertexBuffer(mesh.get(), BufferUsageType::Static);
		_ib = IndexBuffer(mesh.get(), BufferUsageType::Static);

		_material = ResourceManager::Get().GetMaterial(materialName);
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		T::SetVertexAttribPointers();
	}

	void Apply(SceneState &sceneState) override {
		// Material set its own uniforms ..?
		glBindVertexArray(_vao);
		// Draw!
		_vb.Bind();
		_ib.Bind();
		_material->Bind();
		auto shader = _material->GetShader();
		shader->SetUniform("Model", sceneState.GetTransform().GetWorldMatrix());
		shader->SetUniform("View", sceneState.GetCamera()->GetView());
		shader->SetUniform("Projection", *(sceneState.GetProjectionMatrix()));

		glDrawElements(GL_TRIANGLES, _ib.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
	}
};
} // namespace Nimble