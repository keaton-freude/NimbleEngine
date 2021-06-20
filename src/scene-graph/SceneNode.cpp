#include "nimble/scene-graph/SceneNode.h"

#include <optional>

using namespace Nimble;

SceneNode::SceneNode(const std::string &name) : _node_name(name) {
	_id = SceneNode::GenerateID();
}

SceneNode::NodeIdRet SceneNode::AddChild(SceneNode *node) {
	spdlog::debug("Adding node as child to scene graph");

	node->_transform *= _transform;

	_children.push_back(std::unique_ptr<SceneNode>(node));
	return std::make_pair(_children.back().get(), _children.back()->GetID());
}

SceneNode::NodeIdRet SceneNode::AddChild(std::unique_ptr<SceneNode> &&node) {
	spdlog::debug("Adding node as child to scene graph");

	node->_transform *= _transform;

	_children.push_back(std::move(node));
	return std::make_pair(_children.back().get(), _children.back()->GetID());
}

void SceneNode::Visit(SceneState &sceneState) {
	// Visit ourself first, passing ref
	this->Apply(sceneState);

	// Visit each of our children
	for(auto &child : _children) {
		// pass copies
		child->Visit(sceneState);
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
	_transform.Translate(translation);

	for(auto &child : _children) {
		child->PropagateTranslation(translation);
	}
}

void SceneNode::PropagateRotation(const glm::vec3 &axis, float rotation) {
	_transform.Rotate(axis, rotation);

	for(auto &child : _children) {
		child->PropagateRotation(axis, rotation);
	}
}

void SceneNode::PropagateScale(const glm::vec3 &scale) {
	_transform.Scale(scale);

	for(auto &child : _children) {
		child->PropagateScale(scale);
	}
}

void SceneNode::Translate(glm::vec3 translation) {
	PropagateTranslation(translation);
}

void SceneNode::Rotate(glm::vec3 axis, float radians) {
	PropagateRotation(axis, radians);
}

void SceneNode::Scale(glm::vec3 scale) {
	PropagateScale(scale);
}

void SceneNode::SetTranslation(glm::vec3 translation) {
	// Hmm, probably should add a way to propagate a "set transform"
	// series of calls, instead of this hack

	// Undo existing translation
	PropagateTranslation(-_transform.GetTranslation());

	// Now apply the requested translation
	PropagateTranslation(translation);
}

const Transformation &SceneNode::GetTransformation() const {
	return _transform;
}
