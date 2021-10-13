#include "nimble/scene-graph/SceneNode.h"
#include "glm/glm.hpp"

#include <optional>

using namespace Nimble;

SceneNode::SceneNode(const std::string &name) : _node_name(name) {
	_id = SceneNode::GenerateID();
}

SceneNode::NodeIdRet SceneNode::AddChild(SceneNode *node) {
	spdlog::debug("Adding node as child to scene graph");

	node->_global_transform *= _global_transform;

	_children.push_back(std::unique_ptr<SceneNode>(node));
	return std::make_pair(_children.back().get(), _children.back()->GetID());
}

SceneNode::NodeIdRet SceneNode::AddChild(std::unique_ptr<SceneNode> &&node) {
	spdlog::debug("Adding node as child to scene graph");

	node->_global_transform *= _global_transform;

	_children.push_back(std::move(node));
	return std::make_pair(_children.back().get(), _children.back()->GetID());
}

void SceneNode::Visit(SceneState &sceneState) {
	this->Apply(sceneState);

	// Visit each of our children
	for(auto &child : _children) {
		// pass copies
		child->Visit(sceneState);
	}
}

void SceneNode::VisitGui(SceneState &sceneState) {
	if(ImGui::TreeNode(GetNodeName().c_str())) {
		this->DrawGuiElements();
		// Visit ourself first, passing ref
		for(auto &child : _children) {
			child->VisitGui(sceneState);
		}
		ImGui::TreePop();
	}
}

const SceneNode::ChildrenStorageTy &SceneNode::GetChildren() {
	return _children;
}

size_t SceneNode::GetID() const {
	return _id;
}

const std::string &SceneNode::GetNodeName() const {
	return _node_name;
}

size_t SceneNode::GenerateID() {
	static size_t CurrentId = 0;
	CurrentId++;
	return CurrentId;
}

std::optional<SceneNode *> SceneNode::Find(size_t id) {
	// Look through our children for the correct node
	for(auto &child : _children) {
		if(child->GetID() == id) {
			return child.get();
		} else {
			// recurse
			auto found = child->Find(id);
			if(found.has_value()) {
				return found;
			}
		}
	}

	return {};
}

void SceneNode::PropagateTranslation(const glm::vec3 &translation) {
	_global_transform.Translate(translation);

	for(auto &child : _children) {
		child->PropagateTranslation(translation);
	}
}

void SceneNode::PropagateRotation(const glm::quat &rotation) {
	_global_transform.Rotate(rotation);

	for(auto &child : _children) {
		child->PropagateRotation(rotation);
	}
}

void SceneNode::PropagateScale(const glm::vec3 &scale) {
	_global_transform.Scale(scale);

	for(auto &child : _children) {
		child->PropagateScale(scale);
	}
}

void SceneNode::Translate(glm::vec3 translation) {
	PropagateTranslation(translation);
}

void SceneNode::Rotate(const glm::quat &rotation) {
	PropagateRotation(rotation);
}

void SceneNode::Scale(glm::vec3 scale) {
	PropagateScale(scale);
}

void SceneNode::SetTranslation(glm::vec3 translation) {
	_local_transform.SetTranslation(translation);
	// Hmm, probably should add a way to propagate a "set transform"
	// series of calls, instead of this hack

	// Undo existing translation
	Translate(-_global_transform.GetTranslation());

	// Now apply the requested translation
	Translate(translation);
}

void SceneNode::SetScale(glm::vec3 scale) {
	auto transform = scale / _local_transform.GetScale();
	_local_transform.SetScale(scale);
	/*glm::vec3 delta = glm::vec3(1.f) / _global_transform.GetScale();
	Scale(delta);*/

	Scale(transform);
}

void SceneNode::SetRotation(glm::quat rotation) {
	_local_transform.SetRotation(rotation);
	Rotate(glm::inverse(_global_transform.GetRotation()));
	Rotate(rotation);
}

void SceneNode::ResetRotation() {
	_global_transform.SetRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	for(auto &child : _children) {
		child->ResetRotation();
	}
}

const Transformation &SceneNode::GetGlobalTransformation() const {
	return _global_transform;
}

const Transformation &SceneNode::GetLocalTransformation() const {
	return _local_transform;
}
