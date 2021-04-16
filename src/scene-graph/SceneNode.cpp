#include "nimble/scene-graph/SceneNode.h"
#include "spdlog/spdlog.h"

#include <optional>

using namespace Nimble;

SceneNode::SceneNode() {
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

void SceneNode::Visit(SceneState sceneState) {
	// Visit ourself first, passing ref
	this->Apply(sceneState);

	// Visit each of our children
	for(auto &child : _children) {
		// pass copies
		child->Visit(sceneState);
	}
}

size_t SceneNode::GetID() const {
	return _id;
}

size_t SceneNode::GenerateID() {
	static size_t CurrentId = 0;
	return CurrentId++;
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

const Transformation &SceneNode::GetTransformation() const {
	return _transform;
}
