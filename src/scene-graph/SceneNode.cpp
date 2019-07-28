#include "nimble/scene-graph/SceneNode.h"
#include "spdlog/spdlog.h"

#include <optional>

using namespace Nimble;

SceneNode::SceneNode() {
	_id = SceneNode::GenerateID();
}

size_t SceneNode::AddChild(SceneNode *node) {
	spdlog::debug("Adding node as child to scene graph");
	_children.push_back(std::unique_ptr<SceneNode>(node));
	return _children.back()->GetID();
}

size_t SceneNode::AddChild(std::unique_ptr<SceneNode> &&node) {
	spdlog::debug("Adding node as child to scene graph");
	_children.push_back(std::move(node));
	return _children.back()->GetID();
}

void SceneNode::Visit() {
	// Visit ourself first
	this->Apply();

	// Visit each of our children
	for(auto &child : _children) {
		child->Visit();
	}
}

size_t SceneNode::GetID() const {
	return _id;
}

size_t SceneNode::GenerateID() {
	static size_t CurrentId = 0;
	return CurrentId++;
}

const std::optional<SceneNode *const> SceneNode::Find(size_t id) {
	// Look through our children for the correct node
	for(auto &child : _children) {
		if(child->GetID() == id) {
			return child.get();
		} else {
			// recurse
			auto &found = child->Find(id);
			if(found.has_value()) {
				return found;
			}
		}
	}

	return {};
}