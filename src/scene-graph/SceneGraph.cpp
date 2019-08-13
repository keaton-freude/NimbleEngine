#include "nimble/scene-graph/SceneGraph.h"

using namespace Nimble;

SceneGraph::SceneGraph(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<Camera> camera)
: _rootNode(std::make_unique<RootSceneNode>(projectionMatrix, camera)) {
}

void SceneGraph::Render() {
	_rootNode->Render();
}

SceneNode::NodeIdRet SceneGraph::AddChildToRoot(SceneNode *node) {
	return _rootNode->AddChild(node);
}

size_t SceneGraph::AddChild(SceneNode *node, size_t id) {
	// Find the node with the correct id..
	auto parent = Find(id);
	if(!parent.has_value()) {
		throw std::runtime_error(fmt::format("Failed to find parent node for ID {}", id));
	}

	return parent.value()->AddChild(node).second;
}

size_t SceneGraph::AddChild(std::unique_ptr<SceneNode> &&node, size_t id) {
	auto parent = Find(id);
	if(!parent.has_value()) {
		throw std::runtime_error(fmt::format("Failed to find parent node for ID {}", id));
	}

	return parent.value()->AddChild(std::move(node)).second;
}

const std::optional<SceneNode *const> SceneGraph::Find(size_t id) {
	if(_rootNode->GetID() == id)
		return _rootNode.get();

	return _rootNode->Find(id);
}