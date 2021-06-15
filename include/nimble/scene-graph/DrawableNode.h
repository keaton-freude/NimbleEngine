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
#include "nimble/opengl-wrapper/VertexArrayObject.h"
#include "nimble/opengl-wrapper/VertexBuffer.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"

#include "nimble/core/Assert.h"
#include <memory>


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

public:
	// Drawable node from pre-existing resources
	DrawableNode(const IMesh *mesh, std::shared_ptr<Material> material)
	: _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(mesh->GetVao()), _material(material) {
		_vao->Bind();
		_vb.LoadFromMesh(mesh);
		_ib.LoadFromMesh(mesh);
	}

	DrawableNode(const IMesh *mesh, const std::string &materialName)
	: _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(mesh->GetVao()), _material(nullptr) {
		InitFromFilenames(mesh, materialName);
	}

	// Drawable node with resource names
	DrawableNode(const std::string &meshName, const std::string &materialName)
	: _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(nullptr), _material(nullptr) {
		InitFromFilenames(meshName, materialName);
	}

	void Apply(SceneState &sceneState) override {
	}

	std::shared_ptr<VertexArrayObject> GetVAO() const {
		return _vao;
	}

	VertexBuffer &GetVB() {
		return _vb;
	}

	IndexBuffer &GetIB() {
		return _ib;
	}

	std::shared_ptr<Material> GetMaterial() {
		return _material;
	}

	static SceneNodeType SCENE_NODE_TYPE() {
		return SceneNodeType::DRAWABLE;
	}

	SceneNodeType GetSceneNodeType() override {
		return SCENE_NODE_TYPE();
	}

private:
	void InitFromFilenames(const std::string &meshName, const std::string &materialName) {
		// TODO: Fix this hack: Should be somehow computing or requiring the mesh cache key
		auto mesh = ResourceManager::Get().GetMesh(meshName, meshName);
		ASSERT_NOT_NULL(mesh);

		_vao = mesh->GetVao();
		_vao->Bind();
		_vb.LoadFromMesh(mesh.get());
		_ib.LoadFromMesh(mesh.get());
		_vao->SetVertexAttribs();
		_material = ResourceManager::Get().GetMaterial(materialName);
		_vao->Unbind();
		_vb.Unbind();
		_ib.Unbind();
	}


	void InitFromFilenames(const IMesh *mesh, const std::string &materialName) {
		ASSERT_NOT_NULL(mesh);

		_vao = mesh->GetVao();
		_vao->Bind();
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