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
#include "nimble/opengl-wrapper/VertexArrayObject.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"

#include <memory>
#include "nimble/core/Assert.h"


namespace Nimble {

// T is the VertexAttribute type
class DrawableNode : public SceneNode {
private:
	// Require at least a vertex buffer and index buffer
	VertexBuffer _vb;
	IndexBuffer _ib;
	std::shared_ptr<VertexArrayObject> _vao;

	// Material slot, required
	std::shared_ptr<Material> _material;

	// An additional local transform to apply to the global transform
	// before rendering
	Transformation _localTransform;

public:
	// Drawable node from pre-existing resources
	DrawableNode(const IMesh *mesh, std::shared_ptr<Material> material)
	: _vb(BufferUsageType::Static),
	  _ib(BufferUsageType::Static),
	  _vao(mesh->GetVao()),
	  _material(material),
	  _localTransform() {
		_vao->Bind();
		_vb.LoadFromMesh(mesh);
		_ib.LoadFromMesh(mesh);
	}

	DrawableNode(const IMesh *mesh, const std::string& materialName, const Transformation& transform)
	: _vb(BufferUsageType::Static),
	  _ib(BufferUsageType::Static),
	  _vao(mesh->GetVao()),
	  _material(nullptr),
	  _localTransform(transform) {
		InitFromFilenames(mesh, materialName);
	}

	// Drawable node with resource names
	DrawableNode(const std::string &meshName, const std::string &materialName)
	: _vb(BufferUsageType::Static),
	  _ib(BufferUsageType::Static),
	  _vao(nullptr),
	  _material(nullptr),
	  _localTransform() {
		InitFromFilenames(meshName, materialName);
	}

	// Drawable node, resource names & passed in transform
	DrawableNode(const std::string &meshName, const std::string &materialName, const Transformation &transform)
	: _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _material(nullptr), _localTransform(transform) {
		InitFromFilenames(meshName, materialName);
	}

	void Apply(SceneState &sceneState) override {
		_vao->Bind();
		_vb.Bind();
		_ib.Bind();
		_material->Bind();
		auto shader = _material->GetShader();
		auto overallTransform = sceneState.GetTransform() * _localTransform;
		shader->SetUniform("Model", overallTransform.GetWorldMatrix());
		shader->SetUniform("View", sceneState.GetCamera()->GetView());
		shader->SetUniform("Projection", *(sceneState.GetProjectionMatrix()));

		if(sceneState.GetDirectionalLight().enabled && _material->GetReceivesLighting()) {
			shader->SetUniform("lightDirection", sceneState.GetDirectionalLight().direction);
			shader->SetUniform("lightColor", sceneState.GetDirectionalLight().color);
			shader->SetUniform("viewPos", sceneState.GetCamera()->GetPosition());
		}

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_ib.GetNumFaces() * 3), GL_UNSIGNED_INT, 0);
		_vao->Unbind();
	}

private:
	void InitFromFilenames(const std::string &meshName, const std::string &materialName) {
		auto mesh = ResourceManager::Get().GetMesh(meshName);
		_vao = mesh->GetVao();
		_vao->Bind();
		ASSERT_NOT_NULL(mesh);
		_vb.LoadFromMesh(mesh.get());
		_ib.LoadFromMesh(mesh.get());
		_vao->SetVertexAttribs();
		_material = ResourceManager::Get().GetMaterial(materialName);
		_vao->Unbind();
		_vb.Unbind();
		_ib.Unbind();
	}


	void InitFromFilenames(const IMesh* mesh, const std::string& materialName) {
		_vao = mesh->GetVao();
		_vao->Bind();
		ASSERT_NOT_NULL(mesh);
		_vb.LoadFromMesh(mesh);
		_ib.LoadFromMesh(mesh);
		_vao->SetVertexAttribs();
		_material = ResourceManager::Get().GetMaterial(materialName);
		_vao->Unbind();
		_vb.Unbind();
		_ib.Unbind();
	}
};
} // namespace Nimble