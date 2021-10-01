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
#include "nimble/utility/ImGuiUtility.h"
#include <memory>


namespace Nimble {

// T is the VertexAttribute type
class DrawableNode : public SceneNode {
private:
	// Require at least a vertex buffer and index buffer
	VertexBuffer _vb;
	IndexBuffer _ib;
	std::shared_ptr<VertexArrayObject> _vao;
	float _rotation_degrees = 0.0f;
	glm::vec3 _rotation_angle = glm::vec3(0.f, 0.f, 0.f);

	// Material slot, required
	std::shared_ptr<Material> _material;

public:
	// Drawable node from pre-existing resources
	DrawableNode(const IMesh *mesh, std::shared_ptr<Material> material, const std::string &name)
	: SceneNode(name), _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(mesh->GetVao()), _material(material) {
		_vao->Bind();
		_vb.LoadFromMesh(mesh);
		_ib.LoadFromMesh(mesh);
	}

	DrawableNode(const IMesh *mesh, const std::string &materialName, const std::string &name)
	: SceneNode(name), _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(mesh->GetVao()), _material(nullptr) {
		InitFromFilenames(mesh, materialName);
	}

	// Drawable node with resource names
	DrawableNode(const std::string &meshName, const std::string &materialName, const std::string &name)
	: SceneNode(name), _vb(BufferUsageType::Static), _ib(BufferUsageType::Static), _vao(nullptr), _material(nullptr) {
		InitFromFilenames(meshName, materialName);
	}

	void Apply(SceneState &sceneState) override {
		/*if(ImGui::TreeNode(GetNodeName().c_str())) {
			GUI_SLIDER_FLOAT3(newTransformPosition, GetTransformation().GetTranslation(), -50.f, 50.f);
			SetTranslation(newTransformPosition);
			ImGui::TreePop();
		}*/
	}

	void DrawGuiElements() override {
		/*GUI_SLIDER_FLOAT3(translation, GetTransformation().GetTranslation(), -50.f, 50.f);
		SetTranslation(translation);

		auto currRotation = GetTransformation().GetRotation();
		GUI_SLIDER_FLOAT3(rotationAngle, _rotation_angle, -1.f, 1.f);
		_rotation_angle = rotationAngle;
		GUI_SLIDER_FLOAT1(rotationDegrees, _rotation_degrees, 0.f, 360.f);
		_rotation_degrees = rotationDegrees;
		SetRotation(glm::angleAxis(glm::radians(rotationDegrees), rotationAngle));*/

		bool modified = false;

		glm::vec3 translation = GetTransformation().GetTranslation();
		if(ImGui::SliderFloat3("translation", &translation[0], -50.f, 50.f)) {
			modified = true;
		}

		SetTranslation(translation);

		glm::vec3 rotationAngle = _rotation_angle;
		if(ImGui::SliderFloat3("rotationAngle", &rotationAngle[0], -1.f, 1.f)) {
			modified = true;
		}

		_rotation_angle = rotationAngle;

		float rotationDegrees = _rotation_degrees;
		if(ImGui::SliderFloat("rotationDegrees", &rotationDegrees, 0.f, 360.f)) {
			modified = true;
		}

		_rotation_degrees = rotationDegrees;

		if(modified) {
			SetRotation(glm::angleAxis(glm::radians(rotationDegrees), glm::normalize(rotationAngle)));
		}
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