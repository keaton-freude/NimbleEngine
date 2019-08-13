#include "nimble/scene-graph/SceneNode.h"
#include "spdlog/spdlog.h"

#include <optional>

using namespace Nimble;

SceneNode::SceneNode() {
	_id = SceneNode::GenerateID();
}

SceneNode::NodeIdRet SceneNode::AddChild(SceneNode *node) {
	spdlog::debug("Adding node as child to scene graph");
	_children.push_back(std::unique_ptr<SceneNode>(node));
	return std::make_pair(_children.back().get(), _children.back()->GetID());
}

SceneNode::NodeIdRet SceneNode::AddChild(std::unique_ptr<SceneNode> &&node) {
	spdlog::debug("Adding node as child to scene graph");
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